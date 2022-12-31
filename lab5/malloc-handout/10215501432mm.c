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
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)

/* 基本设置 */
#define WSIZE      4
#define DSIZE      8
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount(bytes) */

#define MAX(x, y)  ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(int *)(p))
#define PUT(p, val)  (*(int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)   (GET(p) & ~0x7)
#define GET_ALLOC(p)  (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)  ((char *)(bp) - WSIZE)
#define FTRP(bp)  ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


/* 自定义宏 */

/* 链表宏 - 跳转到PREV/NEXT，修改PREV/NEXT */
#define PREV(bp)    ((void*)(bp))
#define NEXT(bp)    ((void*)(bp) + WSIZE)
#define GETPREV(bp)    (GET(PREV(bp)))
#define GETNEXT(bp)    (GET(NEXT(bp)))
#define LINKPREV(bp)    (((void*)(bp) - GETPREV(bp)))    //注意是减去偏移量
#define LINKNEXT(bp)    (((void*)(bp) + GETNEXT(bp)))
#define PUTPREV(bp,val) (PUT((void*)(bp), val))
#define PUTNEXT(bp,val) (PUT(((void*)(bp) + WSIZE), val))


/* 自定义全局变量 */

/* 堆指针 - 指向序言块 */
static char *heap_listp = 0;


/* 静态函数声明 */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void insert(void *bp);
static void delete(void *bp);


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    /* 1字节对齐块，4字节序言块，1字节尾声块 */
    if ((heap_listp = mem_sbrk(6*WSIZE)) == (void *)-1)
        return -1;
    
    PUT(heap_listp, 0); //对齐块
    PUT(heap_listp + (1*WSIZE), PACK(16, 1)); //序言块头部
    PUT(heap_listp + (2*WSIZE), NULL); //序言块PREV
    PUT(heap_listp + (3*WSIZE), NULL); //序言块NEXT
    PUT(heap_listp + (4*WSIZE), PACK(16, 1)); //序言块脚部
    PUT(heap_listp + (5*WSIZE), PACK(0, 1)); //尾声块头部
    
    /* heap_listp指向序言块 */
    heap_listp += (2*WSIZE);

    /* 扩展堆 */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}

/*
 * extend_heap - 没有符合的空闲块时，请求一个新的空间，返回块指针
 */
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintian alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    
    /* initialize free block header/footer and epilogue header */
    PUT(HDRP(bp), PACK(size, 0));          /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));          /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));  /* New epulogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

/*
 * coalesce - 合并空闲块，同时维护链表
 * bp位置是一个新的节点，已设置好头部与脚部，但未设置PREV与NEXT
 */
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    /* case 1 */
    if (prev_alloc && next_alloc){
        insert(bp);
        return bp;
    }
    /* case 2 */
    else if (prev_alloc && !next_alloc){
        delete(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert(bp);
    }
    /* case 3，向前合并，无需更新链表*/
    else if (!prev_alloc && next_alloc){
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    /* case 4 */
    else{
        delete(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

/*
 * insert - 插入空闲块到序言块后
 */
static void insert(void *bp)
{
    void* hp = heap_listp;
    int val1 = bp - hp;
    if (GETNEXT(hp) != 0)//非空表
    {
        void* firstbp = LINKNEXT(hp);//找到链式栈的栈顶元素位置
        unsigned int val2 = firstbp - bp;
        PUTPREV(firstbp, val2);
        PUTNEXT(bp, val2);
        PUTPREV(bp, val1);
        PUTNEXT(hp, val1);
    }
    else//空表
    {
        PUTNEXT(bp, 0);
        PUTPREV(bp, val1);
        PUTNEXT(hp, val1);
    }
}

/*
 * delete - 从链表中删除空闲块，被删除的空闲块已分配
 */
static void delete(void *bp)
{
    void* pbp = LINKPREV(bp);
    if (GETNEXT(bp) == 0)//表尾
    {
        PUTNEXT(pbp, 0);
        return;
    }
    void* nbp = LINKNEXT(bp);
    unsigned int val = nbp - pbp;
    PUTPREV(nbp, val);
    PUTNEXT(pbp, val);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;       /* Adjusted block size */
    size_t extendsize;  /* Amount to extend heap if no fit */
    char *bp;

    /* ignore spurious requests */
    if (size == 0)
        return NULL;
    
    /* 处理边界情况 */
    if (heap_listp == 0)
        mm_init();
    
    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL){
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;
}

/*
 * find_fit - 最佳适配
 */
static void *find_fit(size_t asize)
{
    void *bp = heap_listp;
	int mini = 9999999;
	void* minbp = bp;
	while(GETNEXT(bp) != 0)
	{
        if (!GET_ALLOC(HDRP(bp)) &&
	(asize <= GET_SIZE(HDRP(bp))) &&
	(GET_SIZE(HDRP(bp)) - asize < 16))
			return bp;
        if (!GET_ALLOC(HDRP(bp)) &&
	(asize <= GET_SIZE(HDRP(bp))) &&
	GET_SIZE(HDRP(bp)) - asize < mini)
        {
            mini = GET_SIZE(HDRP(bp)) - asize;
            minbp = bp;
        }
		bp = LINKNEXT(bp);
	}
    if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))))
		return bp;
	if (minbp != heap_listp)
		return minbp;


    return NULL;
}

/*
 * place - 放置空闲块，剩余部分超出最小块大小时分割
 */
static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));  /* block size */

    delete(bp);

    if ((csize - asize) >= (2*DSIZE)){
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        /* 放置新的已分配块后，指向分割出的空闲块 */
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        insert(bp);
    }
    else{
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if (ptr == 0)
        return;
    
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
    void *new_ptr;
    size_t cpy_size = GET_SIZE(HDRP(ptr));

    if (size == 0){
        mm_free(ptr);
        return 0;
    }
    if (ptr == NULL)
        return mm_malloc(size);

    new_ptr = mm_malloc(size);

    if (!new_ptr)
        return 0;

    /* 将原块中的内容复制到新块 */
    if (size < cpy_size)
        cpy_size = size;
    memcpy(new_ptr, ptr, cpy_size);
    mm_free(ptr);
    return new_ptr;
}