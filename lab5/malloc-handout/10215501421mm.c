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
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))


#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

#define PACK(size, alloc) ((size) | (alloc))

/* 在p位置读写一个字 */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (unsigned int)(val))

// /* 在p位置读写一个指针 */
// #define GET_PTR_VAL(p)  (*(unsigned long *)(p))
// #define SET_PTR(p, ptr) (*(unsigned long *)(p) = (unsigned long)(ptr))

/* 获取p位置的size和alloc信息 */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) ((GET(p) & 0x1))

/* 获得bp块的头部和脚部 */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* 给定有效载荷指针，找到前一块或下一块 指针 */
#define NEXT_BLKP(bp) ( (char*)(bp) + GET_SIZE(( (char*)(bp) - WSIZE )) )
#define PREV_BLKP(bp) ( (char*)(bp) - GET_SIZE(( (char*)(bp) - DSIZE )) )

/* 获得当前\后继\前驱块大小 */
#define CRT_BLKSZ(bp) GET_SIZE(HDRP(bp))
#define NEXT_BLKSZ(bp) GET_SIZE(HDRP(NEXT_BLKP(bp)))
#define PREV_BLKSZ(bp) GET_SIZE(HDRP(PREV_BLKP(bp)))

/* 得到指向该空闲块的前驱后继的指针 */
#define PRED(bp) ((char*)(bp) + WSIZE)
#define SUCC(bp) ((char*)(bp))

/* 获取块链表前驱后继 */
#define PRED_BLKP(bp) (GET(PRED(bp)))
#define SUCC_BLKP(bp) (GET(SUCC(bp)))


// /* 读写指向前驱、后继指针的指针 */
// #define GET_SUCC(p) ((char*)(*(unsigned long *)(p)))
// #define GET_PRED(p) ((char*)(*(unsigned long *)(p + DSIZE)))

/* 大小类总数 */
#define SEG_LEN 15

static char *global_list_start_ptr;
static char *heap_listp;

static int get_index(size_t); // 获得大小类索引
static void insert_free_block(char*);
static void delete_free_block(char*);
static void *extend_heap(size_t);
static void *coalesce(void *);
static void *find_fit(size_t, int);
static void *place(char *bp, size_t asize);
static size_t align_size(size_t size);
/* ======================== Utils ===============================*/

// 获得大小类索引
static int get_index(size_t v) 
{
    // 本质上是位运算的 log 2, O(1)复杂度

    size_t r, shift;
    r = (v > 0xFFFF)   << 4; v >>= r;
    shift = (v > 0xFF) << 3; v >>= shift; r |= shift;
    shift = (v > 0xF)  << 2; v >>= shift; r |= shift;
    shift = (v > 0x3)  << 1; v >>= shift; r |= shift;
                                          r |= (v >> 1);
    // 从 2^4 开始 (空闲块最小 16 bytes)
    int x = (int)r - 4;
    if(x < 0) 
        x = 0;
    if(x >= SEG_LEN) 
        x = SEG_LEN - 1;
    return x;
}

// 插入空闲块
static void insert_free_block(char* fbp)
{
    int seg_index = get_index(CRT_BLKSZ(fbp));
    char *root = global_list_start_ptr + seg_index * WSIZE;

    void *succ = root;

    while(SUCC_BLKP(succ)) {
        succ = (char *)SUCC_BLKP(succ);
        if ((unsigned int)succ >= (unsigned int)fbp) {
            char *tmp = succ;
            // 按照地址顺序插入空闲块
            // PRED_BLKP(succ) <-> fbp <-> succ
            succ = (char *)PRED_BLKP(succ);
            PUT(SUCC(succ), fbp);
            PUT(PRED(fbp), succ);
            PUT(SUCC(fbp), tmp);
            PUT(PRED(tmp), fbp);
            return;
        }
    }

    /* 如果当前大小类无空闲块 或者 在地址分配时当前空闲块地址最大，被分配在末尾 */
    PUT(SUCC(succ), fbp); 
    PUT(PRED(fbp), succ);
    PUT(SUCC(fbp), NULL);
}

// 删除空闲块
static void delete_free_block(char* fbp)
{
    // 如果有后继和前驱
    if (SUCC_BLKP(fbp) && PRED_BLKP(fbp)) {
        // 让待删除结点的前驱的SUCC位置为该结点的后继
        PUT(SUCC(PRED_BLKP(fbp)), SUCC_BLKP(fbp));
        // 让待删除结点的后继的pred位置为该结点的前驱
        PUT(PRED(SUCC_BLKP(fbp)), PRED_BLKP(fbp));
    } else if (PRED_BLKP(fbp)) {
        PUT(SUCC(PRED_BLKP(fbp)), NULL);
    }
    PUT(SUCC(fbp), NULL);
    PUT(PRED(fbp), NULL);
}

// 拓展堆，对齐size，并执行合并，返回bp指针
static void *extend_heap(size_t asize) 
{
    char *bp;

    if ((long)(bp = mem_sbrk(asize)) == -1) {
        return NULL;
    }

    /* 初始化空闲块的头尾和结尾块的头部 */

    PUT(HDRP(bp), PACK(asize, 0)); /* 空闲块头部 */
    PUT(FTRP(bp), PACK(asize, 0)); /* 空闲块尾部 */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0,1)); /* 结尾块头部 */

    return coalesce(bp);
}

// 对bp指向块前后合并，返回bp指针
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    // 获得前一个块是否分配

    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    // 获得后一个块是否已分配

    size_t size = CRT_BLKSZ(bp); // 获得当前块大小

    /* 前后块非空闲 */
    if (prev_alloc && next_alloc) {
        insert_free_block(bp); // 插入该空闲块
        return bp; 
    }

    /* 前块非空闲，后块空闲 */
    if (prev_alloc && !next_alloc) {
        size += NEXT_BLKSZ(bp);  // 合并两个空闲块
        delete_free_block(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        
        // 设置前驱后继为NULL
        PUT(PRED(bp), NULL);
        PUT(SUCC(bp), NULL);
    }

    /* 前块空闲，后块非空闲 */
    if (!prev_alloc && next_alloc) {
        size += PREV_BLKSZ(bp); 
        delete_free_block(PREV_BLKP(bp));

        // 设置新头部和脚部
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));

        bp = PREV_BLKP(bp);
        PUT(PRED(bp), NULL);
        PUT(SUCC(bp), NULL);
    } 

    /* 前块后块都空闲 */
    if (!prev_alloc && !next_alloc) {
        size += NEXT_BLKSZ(bp) + PREV_BLKSZ(bp);
        
        // 删掉前块后块的空闲块
        delete_free_block(PREV_BLKP(bp));
        delete_free_block(NEXT_BLKP(bp));

        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));

        bp = PREV_BLKP(bp);
        PUT(PRED(bp), NULL);
        PUT(SUCC(bp), NULL);
    }

    // 最后把合并的空闲块插入
    insert_free_block(bp);
    return bp;

}

// 寻找适配
static void *find_fit(size_t size, int seg_idx) 
{
    while (seg_idx < SEG_LEN) {
        char *root = global_list_start_ptr + seg_idx * WSIZE;
        char *bp = (char *)SUCC_BLKP(root);
        // 单独寻找
        while (bp) {
            if ((size_t)CRT_BLKSZ(bp) >= size) 
                return bp;
            
            bp = (char *)SUCC_BLKP(bp);
        }
        // 在这类中未找到适合，在更大类中寻找
        seg_idx++;
    }
    return NULL;
}

// 分配块
static void *place(char *bp, size_t asize)
{
    size_t blk_size = CRT_BLKSZ(bp);
    size_t rm_size = blk_size - asize;

    // 如果当前块未分配
    if (!GET_ALLOC(HDRP(bp))) {
        delete_free_block(bp);
    }

    // 如果剩下的块大于16个字节
    if (rm_size >= 2 * DSIZE) {
        if (asize > 64) {
            PUT(HDRP(bp), PACK(rm_size, 0));
            PUT(FTRP(bp), PACK(rm_size, 0));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(asize, 1));
            PUT(FTRP(NEXT_BLKP(bp)), PACK(asize, 1));
            // 合并剩下块
            insert_free_block(bp);
            // 返回真正放置的块
            return NEXT_BLKP(bp);
        }else {
            PUT(HDRP(bp), PACK(asize, 1));
            PUT(FTRP(bp), PACK(asize, 1));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(rm_size, 0));
            PUT(FTRP(NEXT_BLKP(bp)), PACK(rm_size, 0));
            
            coalesce(NEXT_BLKP(bp));
        }
    }else {
        // 不可分隔
        PUT(HDRP(bp), PACK(blk_size, 1));
        PUT(FTRP(bp), PACK(blk_size, 1));
    }
    return bp;
}

 
// align_size - 对块大小进行对齐，留出首尾空间，返回真实分配大小
static size_t align_size(size_t size)
{
    /* 调整块大小 */
    if(size <= DSIZE) return 2*DSIZE;
    else return DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

    // Code Never Went Here
    return 0;
}

/* ========================= main function =======================*/

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if((heap_listp = mem_sbrk((SEG_LEN + 3) * WSIZE)) == (void *)-1)
        // 分配错误
        return -1;
    int i;

    /* 空闲块 */
    for(i = 0; i < SEG_LEN; ++i)
        PUT(heap_listp + i*WSIZE, NULL);	            // 初始化空闲块大小类头指针

    /* 分配块 */
    PUT(heap_listp + (i+0)*WSIZE, PACK(DSIZE, 1));  /* 序言块头部 */
    PUT(heap_listp + (i+1)*WSIZE, PACK(DSIZE, 1));  /* 序言块尾部 */
    PUT(heap_listp + (i+2)*WSIZE, PACK(0, 1));      /* 结尾块头部 */

    global_list_start_ptr = heap_listp;
    heap_listp += (i+1)*WSIZE; // 对齐到起始块有效载荷

    /* 扩展空栈至 CHUNKSIZE bytes */ 
    if(extend_heap(CHUNKSIZE) == NULL)
        //Alloc Error
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    // 调整后的块大小
    int newsize = align_size(size); 

    // 拓展堆大小
    size_t extendsize; 
    
    char *bp;

    if (size == 0) return NULL;

    // 寻找适配
    if ((bp = find_fit(newsize, get_index(newsize))) != NULL) 
        return place(bp, newsize);
    
    // 未找到适配，分配更多堆空间
    extendsize = MAX(newsize, CHUNKSIZE);

    if ((bp = extend_heap(extendsize)) == NULL) 
        return NULL;

    return place(bp, newsize); 
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = CRT_BLKSZ(ptr);
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{

    // 情形1 ptr为空，则相当于malloc
    if (ptr == NULL){ 
        return mm_malloc(size);
    }

    // 情形2 size为0，则相当于free
    if (size == 0){
        mm_free(ptr);
        return NULL;
    }

    // 情形3 重新分配
    size_t asize = align_size(size);
    size_t old_size = CRT_BLKSZ(ptr);


    char *newptr;

    // 如果旧大小等于要分配的大小
    if (old_size == asize) {
        return ptr;
    }

    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t next_size = NEXT_BLKSZ(ptr);
    char *next_bp = NEXT_BLKP(ptr);
    size_t total_size = old_size;

    // 上一块已分配 and 下一块未分配
    if (prev_alloc && !next_alloc && (old_size + next_size >= asize)){
        total_size += next_size;
        delete_free_block(next_bp);
        PUT(HDRP(ptr), PACK(total_size, 1));
        PUT(FTRP(ptr), PACK(total_size, 1));
        place(ptr, total_size);
    }else if (!next_size && asize >= old_size) {
        size_t extend_size = asize - old_size;
        if ((long)(mem_sbrk(extend_size)) == -1) {
            return NULL;
        }
        PUT(HDRP(ptr), PACK(total_size + extend_size, 1));
        PUT(FTRP(ptr), PACK(total_size + extend_size, 1));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));
        place(ptr, asize);
    }else {
        newptr = mm_malloc(asize);
        if (newptr == NULL) {
            return NULL;
        }
        memcpy(newptr, ptr, MIN(old_size, size));
        mm_free(ptr);
        return newptr;
    }
    return ptr;

}














