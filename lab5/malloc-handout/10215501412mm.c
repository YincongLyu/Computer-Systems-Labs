/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "10215501412",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};


#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4 
#define DSIZE 8 
#define CHUNKSIZE (1<<12) //初始空闲块的大小和扩展堆时的默认大小
#define REALLOC_BOUND (1 << 7)
#define CLASS_SIZE 20
#define MINCLASSSIZE 2
#define MINIMUM_BLOCK_SIZE 24 

#define MAX(x,y) ((x) > (y)? (x) : (y))

/*将块的大小和已分配位|起来，存放在头部或脚部*/
#define PACK(size,alloc) ((size)|(alloc))

/*从地址p读或者写一个四字节的数*/
#define PUT(ptr, p) (*((unsigned int *) (ptr)) = (unsigned int) (p))
#define GET(ptr) ((void *) *(unsigned int *) (ptr))

/*当p指向一个头部或者脚部时，获取该部分所标记的块大小或已分配位*/
#define GET_SIZE(p) ((int)GET(p) & ~0x7)
#define GET_ALLOC(p) ((int)GET(p) & 0x1)

/*当bp指向一个块时，获得头部地址或者脚部地址*/
#define HDRP(bp) ((char *)(bp) - WSIZE)//有效载荷块向前4位就是头部
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)//DSIZE的大小是头部和脚部相加的大小，所以要减去                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

/*当bp指向一个块时，获得后一个块地址或者前一个块地址*/
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))//GET_SIZE取到了bp指向块的大小
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))//GET_SIZE取到了bp前一个块的大小

/*当bp指向一个块时，获得前驱和后继块指针*/
#define GET_PRE(bp) ((unsigned int *)(GET(bp)))
#define GET_SUC(bp) ((unsigned int *)(GET((unsigned int *)bp+1)))

/*获得第num个大小类的头指针指向的地址*/
#define GET_HEAD(num) ((unsigned int *)(GET(heap_listp + WSIZE * num)))

/*组织二叉树*/
#define LEFT(bp) ((void *)(bp))
#define RIGHT(bp) ((void *)(bp)+WSIZE)

#define HEIGHT(bp) ((char *)(bp) + 2*WSIZE)

/*读写二叉树*/
#define PUT_LEFT_CHILD(bp,val) (PUT(LEFT(bp),(int)val))
#define PUT_RIGHT_CHILD(bp,val) (PUT(RIGHT(bp),(int)val))
#define GET_LEFT_CHILD(bp) (GET(LEFT(bp)))
#define GET_RIGHT_CHILD(bp) (GET(RIGHT(bp)))

#define GET_HEIGHT(ptr) ((ptr == NULL) ? 0 : *(unsigned int *) (ptr))

/* 扩展堆时的返回值 */
#define DIRECT_RETURN 1  // realloc
#define COALESCE_RETURN 2  // malloc

/* 在空闲块中的放置方式*/
#define ALLOCATE_FROM_BACK 1
#define ALLOCATE_FROM_FRONT 2
#define ALLOCATE_WHOLE_BLOCK 3

/*取块的大小*/
#define GET_HDRP_SIZE(bp) GET_SIZE(HDRP(bp))

static char *heap_listp=0;//堆的起始位置NULL
static void *segregated_avl = NULL;//树表的头
// static unsigned int *my_tree = 0;
// static size_t flag = 0;

static size_t maxsize = 0;            //最大分配size
static size_t minsize = ~(size_t) 0;  //最小分配size


static void insert(void *bp,size_t size);       //插入节点
static void delete(void *bp);       //删除节点

/*操作树*/
static inline void *getTree(int index) {
    /*通过大小类序号获取大小类的root*/
    return (void *) (segregated_avl) + (index * WSIZE);
}

static inline void updateHeight(void *node) {
    /* 更新树的高度 */
    unsigned int leftHeight = GET_HEIGHT(GET(LEFT(node)));
    unsigned int rightHeight = GET_HEIGHT(GET(RIGHT(node)));
    PUT(HEIGHT(node), 1 + MAX(leftHeight, rightHeight));
}

static inline void *getSmallestBlock(void *root) {
    /*获取最小节点*/
    void *left;
    while ((left = GET(LEFT(root))) != NULL) { root = left; }
    return root;
}

static inline void *rightRotate(void *root) {
    /* 右旋 */
    void *leftChild = GET(LEFT(root));
    void *leftRightChild = GET(RIGHT(leftChild));

    PUT(RIGHT(leftChild), root);
    PUT(LEFT(root), leftRightChild);

    updateHeight(root);
    updateHeight(leftChild);

    return leftChild;
}

static inline void *leftRotate(void *root) {
    /* 左旋 */
    void *rightChild = GET(RIGHT(root));
    void *rightLeftChild = GET(LEFT(rightChild));

    PUT(LEFT(rightChild), root);
    PUT(RIGHT(root), rightLeftChild);

    updateHeight(root);
    updateHeight(rightChild);

    return rightChild;
}

static inline void *doubleLeftRotate(void *root) {
    void *right = GET(RIGHT(root));
    PUT(RIGHT(root), rightRotate(right));//右旋右子节点
    return leftRotate(root);//左旋
}

static inline void *doubleRightRotate(void *root) {
    void *left = GET(LEFT(root));
    PUT(LEFT(root), leftRotate(left));//左旋左子节点
    return rightRotate(root);//右旋
}

static inline void *makeBalance(void *root) {
    /* 平衡节点root */
    void *leftChild = GET(LEFT(root));
    void *rightChild = GET(RIGHT(root));

    int leftHeight = (int) GET_HEIGHT(leftChild);
    int rightHeight = (int) GET_HEIGHT(rightChild);

    if (leftHeight - rightHeight == 2) {
        //左子树高
        if (GET_HEIGHT(GET(RIGHT(leftChild))) >
            GET_HEIGHT(GET(LEFT(leftChild)))) {
      //两次旋转
            root = doubleRightRotate(root);
        } else {
            root = rightRotate(root);
        }
    }

    if (rightHeight - leftHeight == 2) {
        //右子树高
        if (GET_HEIGHT(GET(LEFT(rightChild))) >
            GET_HEIGHT(GET(RIGHT(rightChild)))) {
          //两次旋转
          root = doubleLeftRotate(root);
        } else {
          root = leftRotate(root);
        }
    }

    return root;
}

static inline int findTreeIndex(size_t size) {
    /*找到相应的大小类*/
    int index = 0;
    size_t value = MINIMUM_BLOCK_SIZE;
    while (index < CLASS_SIZE - 1) {
        if (size >= value && size < value * 2) { break; }
        index++;
        value <<= 1;
    }
    return index;
}

static inline void initTreeNode(void *ptr, size_t size) {
    /*初始化一个节点，左右子节点设为null，高度设为1*/
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    PUT(RIGHT(ptr), NULL);
    PUT(LEFT(ptr), NULL);
    PUT(HEIGHT(ptr), 1);
}

static inline int defindeAllocateSplitPosition(size_t ptrSize,
                                               size_t targetSize) {
    /*分割块的方式：
    * 如果剩下的大小比最小块要大，则进行分割
    * 如果分配的块大小比较小，从前开始分配
    * 如果分配的块大小比较大，从后开始分配
    * 其他情况下直接分配整个块
    */

    if (ptrSize >= targetSize + MINIMUM_BLOCK_SIZE) {
        if (targetSize > maxsize) {
            //从空闲块的后面开始分配
            maxsize = targetSize;
            if (targetSize < minsize) { minsize = targetSize; }
                return ALLOCATE_FROM_BACK;
        } else if (targetSize < minsize) {
            minsize = targetSize;
            if (targetSize > maxsize) { maxsize = targetSize; }
                return ALLOCATE_FROM_FRONT;
            } else {
            //确定targetsize在中间值的哪一边
                if ((maxsize - targetSize) < (targetSize - minsize)) {
                    //从后面开始分配
                    return ALLOCATE_FROM_BACK;
                } else {
                    return ALLOCATE_FROM_FRONT;
                } 
        }
    } else {
        // 分配整个块
        if (ptrSize > maxsize) { maxsize = ptrSize; }
        if (ptrSize < minsize) { minsize = ptrSize; }

        return ALLOCATE_WHOLE_BLOCK;
    }
}




//合并前后的空闲块
static void *coalesce(void *bp)
{
    /* 获取前后两个块的空闲情况 */
	  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	  size_t size = GET_SIZE(HDRP(bp));//获取当前块的大小
	
	  if (prev_alloc && next_alloc) {				/* 前后都已分配 */
        insert(bp, size);
    } else if (prev_alloc && !next_alloc) {		/* 后一个块空闲 */
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        delete(NEXT_BLKP(bp));
        insert(bp, size);
	  } else if (!prev_alloc && next_alloc) {		/* 前一个块空闲 */
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        delete(PREV_BLKP(bp));
        insert(PREV_BLKP(bp), size);
        bp=PREV_BLKP(bp);
	  } else {	/* 前后都空闲 */
		size += (GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp))));
        delete(NEXT_BLKP(bp));
        delete(PREV_BLKP(bp));
        insert(PREV_BLKP(bp), size);
		bp = PREV_BLKP(bp);
	  } 
    return bp;
}

static void *extend_heap(size_t words, int flags)
{
    char *bp;
    size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if (words <= 0)
        return 0;
    /* 分配偶数个字，保证八字节对齐 */

    if((long) (bp=mem_sbrk(size))==-1)//申请的区域超出最大堆的大小
        return NULL;
    /* 添加空闲块的头部和脚部 */
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));

    /*如果是malloc，采用立即边界合并方式：在每次一个块被释放时就合并所有的相邻块*/
    if (flags == COALESCE_RETURN) { bp = coalesce(bp); }
    return bp;
}

static void *insertFreeBlock_(void *root, void *ptr) {
    if (root == NULL) { return ptr; }

    void *left = LEFT(root);
    void *right = RIGHT(root);

    if (ptr < root) {
        PUT(left, insertFreeBlock_(GET(left), ptr));
    } else if (ptr > root) {
        PUT(right, insertFreeBlock_(GET(right), ptr));
    } 

    updateHeight(root);
    root = makeBalance(root);
    return root;
}
//插入节点
void insert(void *bp,size_t size)
{
    initTreeNode(bp, size);
    int treeIndex = findTreeIndex(size);
    void *tree = getTree(treeIndex);

    PUT(tree, insertFreeBlock_(GET(tree), bp));
}



static void *deleteBlock_(void *root, void *ptr) {
    if (root == NULL) return root;

    void *left = LEFT(root);
    void *right = RIGHT(root);
    void *leftChildAddress = GET(left);
    void *rightChildAddress = GET(right);

    if (ptr < root) {
        PUT(left, deleteBlock_(leftChildAddress, ptr));
    } else if (ptr > root) {
        PUT(right, deleteBlock_(rightChildAddress, ptr));
    } else {
        if (leftChildAddress == NULL && rightChildAddress == NULL) {
            return NULL;
        } else if (leftChildAddress == NULL && rightChildAddress != NULL) {
        root = rightChildAddress;
        } else if (leftChildAddress != NULL && rightChildAddress == NULL) {
        root = leftChildAddress;
        } else {
        void *smallest = getSmallestBlock(GET(right));

        PUT(right, deleteBlock_(GET(right), smallest));
        root = smallest;
        PUT(RIGHT(root), GET(right));
        PUT(LEFT(root), GET(left));
        }
    }

    updateHeight(root);
    root = makeBalance(root);

    return root;
}


//删除节点
void delete(void *bp)
{
	size_t size = GET_SIZE(HDRP(bp));
    int treeIndex = findTreeIndex(size);
    void *tree = getTree(treeIndex);

    PUT(tree, deleteBlock_(GET(tree),bp));
}



static void findFreeBlock_(void *root,
                           size_t targetSize,
                           void **allocateBlock) {
    /* 最优适配 */
    if (root == NULL) return;

    size_t size = GET_SIZE(HDRP(root));
    void *left = GET(LEFT(root));
    void *right = GET(RIGHT(root));

    findFreeBlock_(left, targetSize, allocateBlock);

    if (size >= targetSize) {
        if (*allocateBlock == NULL) {
        *allocateBlock = root;
        } else if ((GET_SIZE(HDRP(*allocateBlock)) - targetSize) >
               (size - targetSize)) {
        *allocateBlock = root;
        }
    }

    findFreeBlock_(right, targetSize, allocateBlock);
}


/* 放置策略 */
static void *place(void *bp, size_t asize)
{
    void *allocateBlock = NULL;
    size_t ptrSize = GET_SIZE(HDRP(bp));

    delete(bp);
    int allocatePosition = defindeAllocateSplitPosition(ptrSize, asize);//确定分割策略
    size_t truncatedSize = ptrSize - asize;
    if (allocatePosition == ALLOCATE_FROM_BACK) {
        insert(bp, truncatedSize);
        allocateBlock = NEXT_BLKP(bp);
        PUT(HDRP(allocateBlock), PACK(asize, 1));
        PUT(FTRP(allocateBlock), PACK(asize, 1));
    } else if (allocatePosition == ALLOCATE_FROM_FRONT) {
        allocateBlock = bp;
        PUT(HDRP(allocateBlock), PACK(asize, 1));
        PUT(FTRP(allocateBlock), PACK(asize, 1));
        insert(NEXT_BLKP(allocateBlock), truncatedSize);
    } else {
      //不分割
        allocateBlock = bp;
        PUT(HDRP(allocateBlock), PACK(ptrSize, 1));
        PUT(FTRP(allocateBlock), PACK(ptrSize, 1));
    }

    return allocateBlock;
}

/* 根据大小类遍历分离树，找到可以分配的空闲块 */
static void *find_fit(size_t asize)
{

    int treeIndex = findTreeIndex(asize);

    void *allocateBlock = NULL;

    for (int i = treeIndex; i < CLASS_SIZE; i++) {
    // 确定大小类
        void *tree = getTree(i);

        findFreeBlock_(GET(tree), asize, &allocateBlock);
        if (allocateBlock != NULL) {//找到可分配的块
            break;
        }
    }

    if (allocateBlock == NULL) {
        return allocateBlock;
    } else {
        return place(allocateBlock, asize);
    }
	
}

static void *reallocplace(void *new_ptr,
                                  size_t targetSize,
                                  size_t oldSize) {
    /*realloc的放置策略*/

    void *allocateBlock = NULL;
    size_t newBlockSize = GET_SIZE(HDRP(new_ptr));
    if (targetSize < oldSize) oldSize = targetSize;
    size_t truncatedSize = newBlockSize - targetSize;
    int allocatePosition = defindeAllocateSplitPosition(newBlockSize, targetSize);

    if (allocatePosition == ALLOCATE_FROM_BACK) {
        //从后分配
        PUT(HDRP(new_ptr), PACK(truncatedSize, 0));
        allocateBlock = NEXT_BLKP(new_ptr);
        memmove(allocateBlock, new_ptr, oldSize);

        PUT(HDRP(allocateBlock), PACK(targetSize, 1));
        PUT(FTRP(allocateBlock), PACK(targetSize, 1));
        insert(new_ptr, truncatedSize);
    } else if (allocatePosition == ALLOCATE_FROM_FRONT) {
        //从前分配
        allocateBlock = new_ptr;
        PUT(HDRP(allocateBlock), PACK(targetSize, 1));
        PUT(FTRP(allocateBlock), PACK(targetSize, 1));
        insert(NEXT_BLKP(allocateBlock), truncatedSize);
    } else {
        //不分割
        allocateBlock = new_ptr;
        PUT(HDRP(allocateBlock), PACK(newBlockSize, 1));
        PUT(FTRP(allocateBlock), PACK(newBlockSize, 1));
    }

    return allocateBlock;
}


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((segregated_avl = mem_sbrk(CLASS_SIZE * WSIZE)) == (void *) -1) {
        /* 分配大小类头表的空间 */
        return -1;
    }

    for (int i = 0; i < CLASS_SIZE; i++) {
        /* 初始化大小类树的头表 */
        void *root = (void *) (segregated_avl) + i * WSIZE;
        PUT(root, NULL);
    }

    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *) -1) {
        // 分配序言块和第一个块的空间
        return -1;
    }

    PUT(heap_listp, 0);                             // 序言块
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  // 头部
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  // 脚部
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));      // 结尾块

    heap_listp += (2 * WSIZE);  // 指向序言块的脚部
    maxsize = 0;
    minsize = ~(size_t) 0;

    return 0;
}
/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    size_t alignedSize =
        MAX(ALIGN(size + SIZE_T_SIZE), MINIMUM_BLOCK_SIZE);  //确定要分配的大小：对齐、大于等于最小块

    void *ptr = NULL;

    if ((ptr = find_fit(alignedSize)) != NULL) {
        // 成功找到

        return ptr;
    } else {
        //找不到合适的块，扩展堆
        size_t extendSize = MAX(alignedSize, CHUNKSIZE);
        if ((ptr = extend_heap(extendSize / WSIZE, COALESCE_RETURN)) == NULL) {
          return NULL;
        }

        ptr = place(ptr, alignedSize);

        return ptr;
    }
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    coalesce(ptr);

}
/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (ptr == NULL) {
        //malloc
        return mm_malloc(size);
    }

    if (size == 0) {
        //free
        mm_free(ptr);
        return NULL;
    }

    void *old_ptr = ptr;
    size_t oldSize = GET_SIZE(HDRP(old_ptr));

    void *new_ptr = NULL;
    size_t newSize = MAX(ALIGN(size + SIZE_T_SIZE), MINIMUM_BLOCK_SIZE);

    if (newSize < oldSize) {//要分配的大小小于原始大小，直接放置
        new_ptr = reallocplace(old_ptr, newSize, oldSize);

        return new_ptr;
    } else if (newSize == oldSize) {//要分配的大小等于原始大小，不需要操作
        return ptr;
    } else {//要分配的大小大于原始大小
        void *nextBlock = NEXT_BLKP(ptr);
        void *prevBlock = PREV_BLKP(ptr);
        int nextAlloc = GET_ALLOC(HDRP(nextBlock));
        int prevAlloc = GET_ALLOC(HDRP(prevBlock));
        size_t nextSize = GET_SIZE(HDRP(nextBlock));
        size_t prevSize = GET_SIZE(HDRP(prevBlock));
        size_t minPrevSize = MAX(minsize, REALLOC_BOUND);

        size_t mergedSize = 0;

        if (!prevAlloc && !nextAlloc) {//前后都是空闲块
            mergedSize = (oldSize + nextSize + prevSize);
            if (mergedSize >= newSize) {// 前后块结合后可以分配
                if (mergedSize - newSize >= minPrevSize) {
                    //剩下部分可以分配一个新块
                    delete(prevBlock);
                    delete(nextBlock);
                    PUT(HDRP(prevBlock), PACK(mergedSize, 0));
                    PUT(FTRP(prevBlock), PACK(mergedSize, 0));
                    memmove(prevBlock, old_ptr, oldSize);
                    new_ptr = reallocplace(prevBlock, newSize, oldSize);

                    return new_ptr;
                } else {
                    mergedSize -= prevSize;
                    if (mergedSize >= newSize) {
                        //只删除下一个块就可以满足分配要求
                        delete(nextBlock);
                        PUT(HDRP(old_ptr), PACK(mergedSize, 0));
                        PUT(FTRP(old_ptr), PACK(mergedSize, 0));
                        new_ptr = reallocplace(old_ptr, newSize, oldSize);

                        return new_ptr;
                    }
                }
            }
        } else if (!prevAlloc && nextAlloc) {//前面一个块空闲
            mergedSize = (oldSize + prevSize);
                //释放前一个块
            if (mergedSize >= newSize && mergedSize - newSize >= minPrevSize) {
                delete(prevBlock);
                PUT(HDRP(prevBlock), PACK(mergedSize, 0));
                PUT(FTRP(prevBlock), PACK(mergedSize, 0));
                memmove(prevBlock, old_ptr, oldSize);
                new_ptr = reallocplace(prevBlock, newSize, oldSize);

                return new_ptr;
            } else if (nextSize == 0) {//后面一个块是结尾块
                //直接扩展堆
                size_t requiredSize = newSize - oldSize;
                size_t requiredWord = (requiredSize % WSIZE == 0)
                                            ? requiredSize / WSIZE
                                            : (requiredSize / WSIZE + 1);
                void *extended = extend_heap(requiredWord, DIRECT_RETURN);
                mergedSize = GET_SIZE(HDRP(extended)) + oldSize;
                PUT(HDRP(old_ptr), PACK(mergedSize, 0));
                PUT(FTRP(old_ptr), PACK(mergedSize, 0));
                new_ptr = reallocplace(old_ptr, newSize, oldSize);

                return new_ptr;
            }
        } else if (prevAlloc && !nextAlloc) {//后一个块空闲
            mergedSize = (oldSize + nextSize);
            if (mergedSize >= newSize) {
                delete(nextBlock);
                PUT(HDRP(old_ptr), PACK(mergedSize, 0));
                PUT(FTRP(old_ptr), PACK(mergedSize, 0));
                new_ptr = reallocplace(old_ptr, newSize, oldSize);

                return new_ptr;
            }
        } else {
            // 前后都已分配
            if (nextSize == 0) {
            //如果后一个块是结尾块
            size_t requiredSize = newSize - oldSize;
            size_t requiredWord = (requiredSize % WSIZE == 0)
                                        ? requiredSize / WSIZE
                                        : (requiredSize / WSIZE + 1);
            void *extended = extend_heap(requiredWord, DIRECT_RETURN);
            mergedSize = GET_SIZE(HDRP(extended)) + oldSize;
            PUT(HDRP(old_ptr), PACK(mergedSize, 0));
            PUT(FTRP(old_ptr), PACK(mergedSize, 0));
            new_ptr = reallocplace(old_ptr, newSize, oldSize);

            return new_ptr;
            }
        }
    }

    /*在指针指向的位置无法分配块，只能malloc*/

    if ((new_ptr = mm_malloc(size)) == NULL) { return NULL; }

    if (size < oldSize) { oldSize = size; }
    memcpy(new_ptr, old_ptr, oldSize);
    mm_free(old_ptr);


    return new_ptr;
}