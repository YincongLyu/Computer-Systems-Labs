/*
 * AVL Tree Implementation
 *
 * 大小类区间：
 * 0: 2^0 to 2^1
 * 1: 2^1 to 2^2
 * 2: 2^2 to 2^3
 * 3: 2^3 to 2^4
 * 4: 2^4 to 2^5
 * 5: 2^5 to 2^6
 * 6: 2^6 to 2^7
 * 7: 2^7 to 2^8
 * 8: 2^8 to 2^9
 * 9: 2^9 to 2^10
 * 10: 2^10 to 2^11
 * 11: 2^11 to 2^12
 * 12: 2^12 to 2^13
 * 13: 2^13 to 2^14
 * 14: 2^14 to 2^15
 * 15: 2^15 to 2^16
 * 16: 2^16 to 2^17
 * 17: 2^17 to 2^18
 * 18: 2^18 to 2^19
 * 19: 2^19 to size_t MAX
 * 每个块的大小都是MINIMUM_BLOCK_SIZE的倍数
 * 
 * 每个node由24位构成
 * Node Begin
 * ------------------------------- 0
 *  Header
 * ------------------------------- 4
 *  Left Child Pointer
 * ------------------------------- 8
 *  Right Child Pointer
 * ------------------------------- 12
 *  Height Value
 * ------------------------------- 16
 *  Pad
 * ------------------------------- 20
 *  Footer
 * ------------------------------- 24
 * Node End
 *
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"
team_t team = {
    /* Team name */
    "He Wenyi",
    /* First member's full name */
    "10215501409",
    /* First member's email address */
    "1776576842@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};
#ifndef DEBUG
#define checkAll()
#else
#define checkAll() mm_check()
#endif
//与之前相似的定义
#define WSIZE 4                 
#define DSIZE 8                 
#define CHUNKSIZE (1 << 12)     // 4096位
#define REALLOC_BOUND (1 << 7)  // 在realloc里一起合并大小小于REALLOC_BOUND的块
#define MINIMUM_BLOCK_SIZE 24   // 最小块大小
#define NUM_TREE 20             // avl树的个数

//extand_heap中的flag
#define DIRECT_RETURN 1  // 特殊情况，不可并直接返回(realloc中)
#define COALESCE_RETURN 2  // 正常在合并后返回

//分隔策略三种情况
#define ALLOCATE_FROM_BACK 1
#define ALLOCATE_FROM_FRONT 2
#define ALLOCATE_WHOLE_BLOCK 3

#define MAX(x, y) ((x > y) ? (x) : (y))

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int *) (p))
#define PUT(p, val) (*(unsigned int *) (p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HEADER(ptr) ((char *) (ptr) -WSIZE)
#define FOOTER(ptr) ((char *) (ptr) + GET_SIZE(HEADER(ptr)) - DSIZE)

#define NEXT_BLOCK(ptr) ((char *) (ptr) + GET_SIZE(((char *) (ptr) -WSIZE)))
#define PREV_BLOCK(ptr) ((char *) (ptr) -GET_SIZE(((char *) (ptr) -DSIZE)))

#define ALIGNMENT 8

#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define SET_PTR(ptr, p) (*((unsigned int *) (ptr)) = (uintptr_t)(p))    //与PUT功能一致，可读性更高
#define GET_PTR(ptr) ((void *) (uintptr_t) GET(ptr))

//取左孩子地址
#define LEFT_CHILD(ptr) ((char *) (ptr))

//取右孩子地址
#define RIGHT_CHILD(ptr) ((char *) (ptr) + WSIZE)

//获取树高
#define HEIGHT(ptr) ((char *) (ptr) + 2 * WSIZE)
#define GET_HEIGHT(ptr) ((ptr == NULL) ? 0 : GET(HEIGHT(ptr)))

//堆返回状态
#define OK 0
#define NOT_ALL_FREE_BLOCKS_IN_TREE 1
#define NOT_ALL_TREES_ARE_BALANCED 2
#define NOT_ALL_FREE_BLOCKS_SORTED_BY_ADDRESS 3
#define NOT_ALL_FREE_BLOCK_MARKED_AS_FREE 4
#define NOT_ALL_TREE_NODE_IN_PROPER_TREE 5
#define NOT_ALL_BLOCKS_BIGGER_THAN_MIN_BLOCK_SIZE 6

static char *heap_listp = NULL;      
static void *segregated_avl = NULL;  //第一个树根节点所在地址
static size_t maxAllocatedSize = 0;            //最大分配块大小
static size_t minAllocatedSize = ~(size_t) 0;  //最小分配块大小

static int checker = 1;  //堆返回情况，默认正常

//二叉树基本操作

//运用incline减少频繁的进出栈花费
//根据index获取根节点
static inline void *getTree(int index) {
  return (void *) (segregated_avl) + (index * WSIZE);
}
//根据节点位置获取树高
static inline void updateHeight(void *node) {
  unsigned int leftHeight = GET_HEIGHT(GET_PTR(LEFT_CHILD(node)));
  unsigned int rightHeight = GET_HEIGHT(GET_PTR(RIGHT_CHILD(node)));

  PUT(HEIGHT(node), 1 + MAX(leftHeight, rightHeight));
}

//获取地址最小的块（取最小值）
static inline void *getSmallestBlock(void *root) {
  void *left;
  while ((left = GET_PTR(LEFT_CHILD(root))) != NULL) { root = left; }
  return root;
}

//右旋
static inline void *rightRotate(void *root) {
  void *leftChild = GET_PTR(LEFT_CHILD(root));
  void *leftRightChild = GET_PTR(RIGHT_CHILD(leftChild));

  SET_PTR(RIGHT_CHILD(leftChild), root);
  SET_PTR(LEFT_CHILD(root), leftRightChild);

  updateHeight(root);
  updateHeight(leftChild);

  return leftChild;
}

//左旋
static inline void *leftRotate(void *root) {
  void *rightChild = GET_PTR(RIGHT_CHILD(root));
  void *rightLeftChild = GET_PTR(LEFT_CHILD(rightChild));

  SET_PTR(LEFT_CHILD(rightChild), root);
  SET_PTR(RIGHT_CHILD(root), rightLeftChild);

  updateHeight(root);
  updateHeight(rightChild);

  return rightChild;
}
//右左双旋转，对root节点的左儿子的右子树进行一次插入（先右旋转再左旋转）
static inline void *doubleLeftRotate(void *root) {
  void *right = GET_PTR(RIGHT_CHILD(root));
  SET_PTR(RIGHT_CHILD(root), rightRotate(right));
  return leftRotate(root);
}
//左右双旋转，对root节点的右儿子的左子树进行一次插入（先左旋转再右旋转）
static inline void *doubleRightRotate(void *root) {
  void *left = GET_PTR(LEFT_CHILD(root));
  SET_PTR(LEFT_CHILD(root), leftRotate(left));
  return rightRotate(root);
}

//平衡操作（关键）
static inline void *makeBalance(void *root) {
  void *leftChild = GET_PTR(LEFT_CHILD(root));
  void *rightChild = GET_PTR(RIGHT_CHILD(root));

  int leftHeight = (int) GET_HEIGHT(leftChild);
  int rightHeight = (int) GET_HEIGHT(rightChild);

  if (leftHeight - rightHeight == 2) {      //如果左边过重（超过2）
    //如果左子树中右边较重，double right rotation
    if (GET_HEIGHT(GET_PTR(RIGHT_CHILD(leftChild))) >     
        GET_HEIGHT(GET_PTR(LEFT_CHILD(leftChild)))) {
        root = doubleRightRotate(root);
    } 
    //否则只用旋转一次
    else {
        root = rightRotate(root);
    }
  }

  if (rightHeight - leftHeight == 2) {      //如果右边过重（超过2）
    //如果右子树中左边较重，double left rotation
    if (GET_HEIGHT(GET_PTR(LEFT_CHILD(rightChild))) >
        GET_HEIGHT(GET_PTR(RIGHT_CHILD(rightChild)))) {
      root = doubleLeftRotate(root);
    } 
    //否则只用旋转一次
    else {
      root = leftRotate(root);
    }
  }
  return root;
}

//根据size找对对应树的index
static inline int findTreeIndex(size_t size) {
  int index = 0;
  size_t value = MINIMUM_BLOCK_SIZE;
  while (index < NUM_TREE - 1) {
    if (size >= value && size < value * 2) { break; }
    index++;
    value <<= 1;
  }
  return index;
}

//初始化node
static inline void initTreeNode(void *ptr, size_t size) {
  //size过小，异常
  if (size < MINIMUM_BLOCK_SIZE) {
    fprintf(stderr,
            "Block size (%zu) should be greater or equal than %d\n",
            size,
            MINIMUM_BLOCK_SIZE);
    exit(1);
  }
  //初始化空闲块头部、脚部、左右孩子、高度
  PUT(HEADER(ptr), PACK(size, 0));
  PUT(FOOTER(ptr), PACK(size, 0));
  SET_PTR(RIGHT_CHILD(ptr), NULL);
  SET_PTR(LEFT_CHILD(ptr), NULL);
  PUT(HEIGHT(ptr), 1);
}
//确定分隔策略是从前向后、从后向前、还是整块分配
static inline int defindeAllocateSplitPosition(size_t ptrSize,
                                               size_t targetSize) {
  //如果分隔后，剩余空间还能放下其他空闲块
  if (ptrSize >= targetSize + MINIMUM_BLOCK_SIZE) {
    if (targetSize > maxAllocatedSize) {   //如果targetSize接近maxAllocatedSize,就从后往前
      maxAllocatedSize = targetSize;
      if (targetSize < minAllocatedSize) { minAllocatedSize = targetSize; }
      return ALLOCATE_FROM_BACK;
    } else if (targetSize < minAllocatedSize) {//如果targetSize接近minAllocatedSize,就从前往后
      minAllocatedSize = targetSize;
      if (targetSize > maxAllocatedSize) { maxAllocatedSize = targetSize; }
      return ALLOCATE_FROM_FRONT;
    } else {
      if ((maxAllocatedSize - targetSize) < (targetSize - minAllocatedSize)) {
        return ALLOCATE_FROM_BACK;
      } else {
        return ALLOCATE_FROM_FRONT;
      }
    }
  } 
  //否组整块分配给ptr
  else {
    if (ptrSize > maxAllocatedSize) { maxAllocatedSize = ptrSize; }
    if (ptrSize < minAllocatedSize) { minAllocatedSize = ptrSize; }

    return ALLOCATE_WHOLE_BLOCK;
  }
}

//自定义函数
static void *extendHeap(size_t words, int flags);
static void *coalesce(void *ptr);
static void insertFreeBlock(void *ptr, size_t size);
static void *insertFreeBlock_(void *root, void *ptr);

static void *findFreeBlock(size_t targetSize);
static void findFreeBlock_(void *root,
                           size_t targetSize,
                           void **allocateBlock);

static void *splitAndPlace(void *ptr, size_t targetSize);
static void *reallocSplitAndPlace(void *new_ptr,
                                  size_t newSize,
                                  size_t oldSize);

static void deleteBlock(void *ptr);
static void *deleteBlock_(void *root, void *ptr);

//自定义堆检查函数
static void checkAllFreeBlocksInTree();
static void checkAllFreeBlocksInTree_(void *root, void *ptr);
static void checkAllTreesBalanced();
static void checkAllTreesBalanced_(void *root);
static void checkAllTreesOrderedByAddress();
static void checkAllTreesOrderedByAddress_(void *root, void *previous);
static void checkEveryFreeBlockMarkedAsFree();
static void checkEveryFreeBlockMarkedAsFree_(void *root);
static void checkEveryTreeNodeInProperTree();
static void checkEveryTreeNodeInProperTree_(void *root,
                                            size_t minSize,
                                            size_t maxSize);
static void checkEveryBlocksBiggerThanMIN_BLOCK_SIZE();
static void printTree();
static void printTree_(void *root, unsigned int indent);


int mm_init(void) {
  /*
   * Initialize global static pointers
   * 	heap_listp: start point of heap
   * 	segregated_avl: start point of segregated avl roots
   *
   * Initialize global variable
   *  maxAllocatedSize = 0
   *  minAllocatedSize = ~(size_t) 0
   */
  //初始化segregated_avl数组
  if ((segregated_avl = mem_sbrk(NUM_TREE * WSIZE)) == (void *) -1) {
    return -1;
  }
  //初始化segregated_avl数组中的每个树
  for (int i = 0; i < NUM_TREE; i++) {
    void *root = (void *) (segregated_avl) + i * WSIZE;
    SET_PTR(root, NULL);
  }

  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *) -1) {
    return -1;
  }

  PUT(heap_listp, 0);                             // 对齐块
  PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  // 序言块
  PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  
  PUT(heap_listp + (3 * WSIZE), PACK(0, 1));      // 结尾快

  heap_listp += (2 * WSIZE);  

  maxAllocatedSize = 0;
  minAllocatedSize = ~(size_t) 0;     //注意minAllocatedSize初始化方式

  return 0;
}


void *mm_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  size_t alignedSize =
      MAX(ALIGN(size + SIZE_T_SIZE), MINIMUM_BLOCK_SIZE);  // 对齐

  void *ptr = NULL;

  if ((ptr = findFreeBlock(alignedSize)) != NULL) {
    checkAll();
    return ptr;
  } else {      //空间不够，扩栈
    size_t extendSize = MAX(alignedSize, CHUNKSIZE);
    // 总体空间不够，失败
    if ((ptr = extendHeap(extendSize / WSIZE, COALESCE_RETURN)) == NULL) {
      return NULL;
    }

    ptr = splitAndPlace(ptr, alignedSize);
    checkAll();
    return ptr;
  }
}

void mm_free(void *ptr) {
  size_t size = GET_SIZE(HEADER(ptr));
  PUT(HEADER(ptr), PACK(size, 0));
  PUT(FOOTER(ptr), PACK(size, 0));

  coalesce(ptr);
  checkAll();
}

void *mm_realloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    return mm_malloc(size);
  }

  if (size == 0) {
、    mm_free(ptr);
    return NULL;
  }

  void *old_ptr = ptr;
  size_t oldSize = GET_SIZE(HEADER(old_ptr));

  void *new_ptr = NULL;
  size_t newSize = MAX(ALIGN(size + SIZE_T_SIZE), MINIMUM_BLOCK_SIZE);

  if (newSize < oldSize) {
    new_ptr = reallocSplitAndPlace(old_ptr, newSize, oldSize);
    checkAll();
    return new_ptr;
  } else if (newSize == oldSize) {
    return ptr;
  } else {
    void *nextBlock = NEXT_BLOCK(ptr);
    void *prevBlock = PREV_BLOCK(ptr);
    int nextAlloc = GET_ALLOC(HEADER(nextBlock));
    int prevAlloc = GET_ALLOC(HEADER(prevBlock));
    size_t nextSize = GET_SIZE(HEADER(nextBlock));
    size_t prevSize = GET_SIZE(HEADER(prevBlock));
    //当合并后块小于minAllocatedSize或REALLOC_BOUND，强制不合并
    size_t minPrevSize = MAX(minAllocatedSize, REALLOC_BOUND);  

    size_t mergedSize = 0;
    //预合并，提升效率
    if (!prevAlloc && !nextAlloc) {     //前后都空
      mergedSize = (oldSize + nextSize + prevSize);
      if (mergedSize >= newSize) {
        if (mergedSize - newSize >= minPrevSize) {
          //合并前后块
          deleteBlock(prevBlock);
          deleteBlock(nextBlock);
          PUT(HEADER(prevBlock), PACK(mergedSize, 0));
          PUT(FOOTER(prevBlock), PACK(mergedSize, 0));
          memmove(prevBlock, old_ptr, oldSize);
          new_ptr = reallocSplitAndPlace(prevBlock, newSize, oldSize);
          checkAll();
          return new_ptr;
        } else {
          mergedSize -= prevSize;
          //不合并前块，合并后块
          if (mergedSize >= newSize) {
            deleteBlock(nextBlock);
            PUT(HEADER(old_ptr), PACK(mergedSize, 0));
            PUT(FOOTER(old_ptr), PACK(mergedSize, 0));
            new_ptr = reallocSplitAndPlace(old_ptr, newSize, oldSize);
            checkAll();
            return new_ptr;
          }
        }
      }
    } else if (!prevAlloc && nextAlloc) {       //前空后不空   
      mergedSize = (oldSize + prevSize);
      if (mergedSize >= newSize && mergedSize - newSize >= minPrevSize) {
        deleteBlock(prevBlock);
        PUT(HEADER(prevBlock), PACK(mergedSize, 0));
        PUT(FOOTER(prevBlock), PACK(mergedSize, 0));
        memmove(prevBlock, old_ptr, oldSize);
        new_ptr = reallocSplitAndPlace(prevBlock, newSize, oldSize);
        checkAll();
        return new_ptr;
      } else if (nextSize == 0) {       //nextSize为0，意味着触碰到了结尾块，扩栈
        size_t requiredSize = newSize - oldSize;
        size_t requiredWord = (requiredSize % WSIZE == 0)       //对齐
                                  ? requiredSize / WSIZE
                                  : (requiredSize / WSIZE + 1);
        void *extended = extendHeap(requiredWord, DIRECT_RETURN);
        mergedSize = GET_SIZE(HEADER(extended)) + oldSize;
        PUT(HEADER(old_ptr), PACK(mergedSize, 0));
        PUT(FOOTER(old_ptr), PACK(mergedSize, 0));
        new_ptr = reallocSplitAndPlace(old_ptr, newSize, oldSize);
        checkAll();
        return new_ptr;
      }
    } else if (prevAlloc && !nextAlloc) {       //前不空后空
      mergedSize = (oldSize + nextSize);
      if (mergedSize >= newSize) {
        deleteBlock(nextBlock);
        PUT(HEADER(old_ptr), PACK(mergedSize, 0));
        PUT(FOOTER(old_ptr), PACK(mergedSize, 0));
        new_ptr = reallocSplitAndPlace(old_ptr, newSize, oldSize);
        checkAll();
        return new_ptr;
      }
    } else {        //前后都不空    
      if (nextSize == 0) {
        //nextSize为0，意味着触碰到了结尾块，扩栈
        size_t requiredSize = newSize - oldSize;
        size_t requiredWord = (requiredSize % WSIZE == 0)
                                  ? requiredSize / WSIZE
                                  : (requiredSize / WSIZE + 1);
        void *extended = extendHeap(requiredWord, DIRECT_RETURN);
        mergedSize = GET_SIZE(HEADER(extended)) + oldSize;
        PUT(HEADER(old_ptr), PACK(mergedSize, 0));
        PUT(FOOTER(old_ptr), PACK(mergedSize, 0));
        new_ptr = reallocSplitAndPlace(old_ptr, newSize, oldSize);
        checkAll();
        return new_ptr;
      }
    }
  }
  //现有块及前后无法放下，delete现有块，重新mm_malloc
  if ((new_ptr = mm_malloc(size)) == NULL) { return NULL; }

  if (size < oldSize) { oldSize = size; }
  memcpy(new_ptr, old_ptr, oldSize);
  mm_free(old_ptr);

  checkAll();
  return new_ptr;
}
static void *extendHeap(size_t words, int flags) {
  void *ptr;
  size_t size;

  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;     //对齐
  //空间不足
  if ((long) (ptr = mem_sbrk(size)) == -1) {
    return NULL;
  }

  PUT(HEADER(ptr), PACK(size, 0));
  PUT(FOOTER(ptr), PACK(size, 0));
  PUT(HEADER(NEXT_BLOCK(ptr)), PACK(0, 1));
  //flag为COALESCE_RETURN直接返回
  if (flags == COALESCE_RETURN) { ptr = coalesce(ptr); }
  checkAll();
  //flag为DIRECT_RETURN直接返回
  return ptr;
}

static void *coalesce(void *ptr) {
  size_t isPrevBlockAllocated = GET_ALLOC(FOOTER(PREV_BLOCK(ptr)));
  size_t isNextBlockAllocated = GET_ALLOC(HEADER(NEXT_BLOCK(ptr)));

  char *previousBlock = NULL;
  char *nextBlock = NULL;

  size_t mergedSize = GET_SIZE(HEADER(ptr));

  if (isPrevBlockAllocated && isNextBlockAllocated) {   //前后都不空
    insertFreeBlock(ptr, mergedSize);
  } else if (!isPrevBlockAllocated && isNextBlockAllocated) {   //前空后不空
    previousBlock = PREV_BLOCK(ptr);
    mergedSize += GET_SIZE(HEADER(previousBlock));
    deleteBlock(previousBlock);
    insertFreeBlock(previousBlock, mergedSize);
    ptr = previousBlock;
  } else if (isPrevBlockAllocated && !isNextBlockAllocated) {   //前不空后空
    nextBlock = NEXT_BLOCK(ptr);
    mergedSize += GET_SIZE(HEADER(nextBlock));
    deleteBlock(nextBlock);
    insertFreeBlock(ptr, mergedSize);
  } else {      //前后都空
    previousBlock = PREV_BLOCK(ptr);
    mergedSize += GET_SIZE(HEADER(previousBlock));
    nextBlock = NEXT_BLOCK(ptr);
    mergedSize += GET_SIZE(HEADER(nextBlock));

    deleteBlock(nextBlock);
    deleteBlock(previousBlock);
    insertFreeBlock(previousBlock, mergedSize);
    ptr = previousBlock;
  }

  checkAll();
  return ptr;
}
//辅助函数，向树中插入块时，初始化块
static void insertFreeBlock(void *ptr, size_t size) {

  initTreeNode(ptr, size);
  int treeIndex = findTreeIndex(size);
  void *tree = getTree(treeIndex);

  SET_PTR(tree, insertFreeBlock_(GET_PTR(tree), ptr));
}
//向树中插入块
static void *insertFreeBlock_(void *root, void *ptr) {
  if (root == NULL) { return ptr; }

  void *left = LEFT_CHILD(root);
  void *right = RIGHT_CHILD(root);

  if (ptr < root) {
    SET_PTR(left, insertFreeBlock_(GET_PTR(left), ptr));
  } else if (ptr > root) {
    SET_PTR(right, insertFreeBlock_(GET_PTR(right), ptr));
  } else {
  }

  updateHeight(root);
  root = makeBalance(root);
  return root;
}

//寻找放置块的位置
static void *findFreeBlock(size_t targetSize) {
  /*
   * Find free block to allocate
   * free block's size should be >= targetSize
   * this is wrapper of real find function
   */
  int treeIndex = findTreeIndex(targetSize);

  void *allocateBlock = NULL;

  for (int i = treeIndex; i < NUM_TREE; i++) {
    // start to find from treeIndex
    void *tree = getTree(i);

    findFreeBlock_(GET_PTR(tree), targetSize, &allocateBlock);
    if (allocateBlock != NULL) {
      /* found! */
      break;
    }
  }

  if (allocateBlock == NULL) {
    return allocateBlock;
  } else {
    return splitAndPlace(allocateBlock, targetSize);
  }
}
//辅助函数，遍历树
static void findFreeBlock_(void *root,
                           size_t targetSize,
                           void **allocateBlock) {
  if (root == NULL) return;
//中序遍历，最佳适配
  size_t size = GET_SIZE(HEADER(root));
  void *left = GET_PTR(LEFT_CHILD(root));
  void *right = GET_PTR(RIGHT_CHILD(root));

  findFreeBlock_(left, targetSize, allocateBlock);

  if (size >= targetSize) {
    if (*allocateBlock == NULL) {
      *allocateBlock = root;
    } else if ((GET_SIZE(HEADER(*allocateBlock)) - targetSize) >
               (size - targetSize)) {
      *allocateBlock = root;
    }
  }

  findFreeBlock_(right, targetSize, allocateBlock);
}
//为realloc执行分隔与放置
static void *reallocSplitAndPlace(void *new_ptr,
                                  size_t targetSize,
                                  size_t oldSize) {
  //new_ptr不在avl树中
  void *allocateBlock = NULL;
  size_t newBlockSize = GET_SIZE(HEADER(new_ptr));
  //对于new_ptr,oldSize不能舍弃
  if (targetSize < oldSize) oldSize = targetSize;
  size_t truncatedSize = newBlockSize - targetSize;
  int allocatePosition =
      defindeAllocateSplitPosition(newBlockSize, targetSize);

  if (allocatePosition == ALLOCATE_FROM_BACK) {
    //从后开始
    //注意先计算头部
    PUT(HEADER(new_ptr), PACK(truncatedSize, 0));
    allocateBlock = NEXT_BLOCK(new_ptr);

    //可以重叠，用memmove,而不是memcpy
    memmove(allocateBlock, new_ptr, oldSize);

    PUT(HEADER(allocateBlock), PACK(targetSize, 1));
    PUT(FOOTER(allocateBlock), PACK(targetSize, 1));
    insertFreeBlock(new_ptr, truncatedSize);
  } else if (allocatePosition == ALLOCATE_FROM_FRONT) {
    //从前开始，不用memmove
    allocateBlock = new_ptr;
    PUT(HEADER(allocateBlock), PACK(targetSize, 1));
    PUT(FOOTER(allocateBlock), PACK(targetSize, 1));
    insertFreeBlock(NEXT_BLOCK(allocateBlock), truncatedSize);
  } else {
    //整体分配
    allocateBlock = new_ptr;
    PUT(HEADER(allocateBlock), PACK(newBlockSize, 1));
    PUT(FOOTER(allocateBlock), PACK(newBlockSize, 1));
  }

  return allocateBlock;
}
//分隔与放置
static void *splitAndPlace(void *ptr, size_t targetSize) {
  void *allocateBlock = NULL;
  size_t ptrSize = GET_SIZE(HEADER(ptr));

  deleteBlock(ptr);
  int allocatePosition = defindeAllocateSplitPosition(ptrSize, targetSize);
  size_t truncatedSize = ptrSize - targetSize;
  //从后开始
  if (allocatePosition == ALLOCATE_FROM_BACK) {
    insertFreeBlock(ptr, truncatedSize);
    allocateBlock = NEXT_BLOCK(ptr);
    PUT(HEADER(allocateBlock), PACK(targetSize, 1));
    PUT(FOOTER(allocateBlock), PACK(targetSize, 1));
  //从前开始
  } else if (allocatePosition == ALLOCATE_FROM_FRONT) {
    allocateBlock = ptr;
    PUT(HEADER(allocateBlock), PACK(targetSize, 1));
    PUT(FOOTER(allocateBlock), PACK(targetSize, 1));
    insertFreeBlock(NEXT_BLOCK(allocateBlock), truncatedSize);
  } else {
    //整个分配
    allocateBlock = ptr;
    PUT(HEADER(allocateBlock), PACK(ptrSize, 1));
    PUT(FOOTER(allocateBlock), PACK(ptrSize, 1));
  }

  return allocateBlock;
}

//删除块
static void deleteBlock(void *ptr) {
  size_t size = GET_SIZE(HEADER(ptr));
  int treeIndex = findTreeIndex(size);
  void *tree = getTree(treeIndex);

  SET_PTR(tree, deleteBlock_(GET_PTR(tree), ptr));
}
//辅助函数，删除快
static void *deleteBlock_(void *root, void *ptr) {
  if (root == NULL) return root;
  void *left = LEFT_CHILD(root);
  void *right = RIGHT_CHILD(root);
  void *leftChildAddress = GET_PTR(left);
  void *rightChildAddress = GET_PTR(right);

  if (ptr < root) {
    SET_PTR(left, deleteBlock_(leftChildAddress, ptr));
  } else if (ptr > root) {
    SET_PTR(right, deleteBlock_(rightChildAddress, ptr));
  } else {
    if (leftChildAddress == NULL && rightChildAddress == NULL) {
      return NULL;
    } else if (leftChildAddress == NULL && rightChildAddress != NULL) {
      root = rightChildAddress;
    } else if (leftChildAddress != NULL && rightChildAddress == NULL) {
      root = leftChildAddress;
    } else {
      //如果左右孩子都为空
      void *smallest = getSmallestBlock(GET_PTR(right));
      //把剩余结点接到smallest上
      SET_PTR(right, deleteBlock_(GET_PTR(right), smallest));
      root = smallest;
      SET_PTR(RIGHT_CHILD(root), GET_PTR(right));
      SET_PTR(LEFT_CHILD(root), GET_PTR(left));
    }
  }

  updateHeight(root);
  root = makeBalance(root);

  return root;
}


