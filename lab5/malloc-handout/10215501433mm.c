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
    "bouboo",
    /* First member's full name */
    "zwx",
    /* First member's email address */
    "475174920@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/*对齐8*/
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/*单字4，双字8*/
#define WSIZE         4
#define DSIZE         8
#define CHUNKSIZE    (1 << 10)  /* 堆扩展时默认大小 */

#define MAX(x, y)    ((x) > (y) ? (x) : (y))

/*头部的最后三位存放块的状态，其余表示块的大小，其中最后一位表示分配的状态*/
/*定义PACK函数改变size的最后一位*/
#define PACK(size, alloc)    ((size) | (alloc))

/* Read and write a word at address p. */
/*字为4字节，将地址p作为一个int型指针，取值、赋值*/
#define GET(p)         (*(unsigned int *)(p))
#define PUT(p, val)    (*(unsigned int *)(p) = (val))

/* Get and Set pointer value by ptr at address p.*/
/* 64 位指针为 8 字节，所以用long，且取下一处时也加8*/
#define GET_PTR(p)         (*(unsigned long *)(p))
#define SET_PTR(p, ptr)    (*(unsigned long *)(p) = (unsigned long)(ptr))

/* Read and write pred and succ pointer at address p */
#define GET_PRED(p)         ((char *)(*(unsigned long *)(p)))
#define GET_SUCC(p)         ((char *)(*(unsigned long *)(p + DSIZE))) /* PRED指针大小为 8 */
/*赋值*/
#define SET_PRED(p, ptr)    (SET_PTR((char *)(p), ptr))
#define SET_SUCC(p, ptr)    (SET_PTR(((char *)(p) + DSIZE), ptr))

/*获得分配状态*/
#define GET_SIZE(p)     (GET(p) & (~0x7))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* bp指向第一个有效载荷(payload)字节, 前一个字节为头部，头部为单字4字节（WSIZE）
*p加上块的大小后得到脚部，块的大小时包含头部和脚部的，所以需要减去8字节（DSIZE）
 */
#define HDRP(bp)    ((char *)(bp) - WSIZE) 
#define FTRP(bp)    ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 

/* bp同上，指向当前块的第一个payload */
#define NEXT_BLKP(bp)    ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))/*获得下一个块的第一个payload*/
#define PREV_BLKP(bp)    ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) /* 获得上一个块的脚部 */

static char *free_list_headp; /*表头*/
static char *free_list_tailp;/*表尾*/

static void *coalesce(void *ptr); 
static void *extend_heap(size_t words); 
static void *find_fit(size_t asize); 
static void place(void *bp, size_t asize); 

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((free_list_headp = mem_sbrk(5 * DSIZE)) == (void *)-1)/*mem_sbrk返回指向旧堆尾部的指针*/
    {
        return -1;
    }

    PUT(free_list_headp, PACK(0, 0)); /* 为了对齐 8 */
    
    /* free_list_headp的block有 24 字节，头部脚步8字节，payload16字节，
     * 序言块
     */
    PUT(free_list_headp + WSIZE, PACK(24, 1));
    free_list_headp += DSIZE;
    free_list_tailp = NEXT_BLKP(free_list_headp);
    SET_PRED(free_list_headp, NULL);
    SET_SUCC(free_list_headp, free_list_tailp);
    PUT(free_list_headp + (2 * DSIZE), PACK(24, 1)); /* free_list_headp的footer */
    
    /* 结尾块 */
    PUT(HDRP(free_list_tailp), PACK(0, 1));
    SET_PRED(free_list_tailp, free_list_headp);

    if (extend_heap((7*DSIZE) / WSIZE) == NULL)
    {
        return -1;
    }
    return 0;
}

static void *extend_heap(size_t words)
{
    //char *bp;
    char *ptr;
    size_t size;

    size = (words % 2) ? (words + 1)*WSIZE : words * WSIZE; /* 使得size为 8 的倍数 */
    if ((long)(mem_sbrk(size)) == -1) /* extend the heap *//*意思是分配失败*/
    {
        return NULL;
    }
    ptr = free_list_tailp;

    /* ptr指向扩展后新区域的第一个payload字节
     * 设置状态为0
     */
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    free_list_tailp = NEXT_BLKP(ptr);
    SET_SUCC(ptr, free_list_tailp);

    PUT(HDRP(free_list_tailp), PACK(0, 1));
    SET_PRED(free_list_tailp, ptr);
    return coalesce(ptr);
}

/* 合并 */
static void *coalesce(void *ptr)
{
    /* 前后块的分配状态 */
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));

    /*当前块的大小*/
    size_t size = GET_SIZE(HDRP(ptr));

    void *prev_ptr = PREV_BLKP(ptr);
    void *next_ptr = NEXT_BLKP(ptr);
    
    /*开始四种状态的判断*/
    if (prev_alloc && next_alloc) /* 前面的块和后面的块都是已分配的 */
    {
        return ptr;/*无需合并，直接返回*/
    }
    else if (prev_alloc && !next_alloc)    /* 前面的块是已分配的，后面的块是空闲的 */
    {
        size += GET_SIZE(HDRP(next_ptr));  /*与下一个块的大小合并*/
        /*改变前后块的指针*/
        SET_SUCC(ptr, GET_SUCC(next_ptr));
        SET_PRED(GET_SUCC(next_ptr), ptr);

        PUT(HDRP(ptr), PACK(size, 0));
        PUT(FTRP(next_ptr), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc) /* 前面的块是空闲的，后面的块是已分配的 */
    {
        size += GET_SIZE(HDRP(prev_ptr));

        SET_SUCC(prev_ptr, GET_SUCC(ptr));
        SET_PRED(GET_SUCC(ptr), prev_ptr);

        PUT(HDRP(prev_ptr), PACK(size, 0));
        PUT(FTRP(ptr), PACK(size, 0));

        ptr = prev_ptr;
    }
    else if (!prev_alloc && !next_alloc) /* 前面和后面的块都是空闲的 */
    {
        size += GET_SIZE(HDRP(prev_ptr)) + GET_SIZE(HDRP(next_ptr));
        
        SET_SUCC(prev_ptr, GET_SUCC(next_ptr));
        SET_PRED(GET_SUCC(next_ptr), prev_ptr);

        PUT(HDRP(prev_ptr), PACK(size, 0));
        PUT(FTRP(next_ptr), PACK(size, 0));
        
        ptr = prev_ptr;
    }
    return ptr;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extend_size;
    char *bp;

    /* 调整大小，与8对齐 */
    if (size == 0) 
    {
        return NULL;
    }
    if (size <= 2*DSIZE) /* 最小块有 24 字节，所以调整最小为24 */
    {
        asize = 3 * DSIZE;
    }
    else /* 大于24也要满足和8对齐 */
    {
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
    }

    if ((bp = find_fit(asize)) != NULL) /* 找到合适块 */
    {
        place(bp, asize);
        return bp;
    }
  
    extend_size = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extend_size / WSIZE)) == NULL)
    {
        return NULL;
    }
    place(bp, asize);
    return bp;
}

static void *find_fit(size_t asize)
{
    void *p;

    /*遍历 */
    for (p = GET_SUCC(free_list_headp); p != free_list_tailp; p = GET_SUCC(p))
    {
        if (asize <= GET_SIZE(HDRP(p)))
        {
            return p;
        }
    }
    return NULL; 
}


static void place(void *bp, size_t asize)
{
    size_t size = GET_SIZE(HDRP(bp));
    void *ptr;

    if ((size - asize) >= 3*DSIZE) /* 剩余大于 24 字节要分割 */
    {
        PUT(HDRP(bp), PACK(asize, 1)); /* 分配尺寸，修改状态 */
        PUT(FTRP(bp), PACK(asize, 1));

        ptr = NEXT_BLKP(bp);
        SET_SUCC(ptr, GET_SUCC(bp));
        SET_PRED(ptr, GET_PRED(bp));
        SET_SUCC(GET_PRED(bp), ptr);
        SET_PRED(GET_SUCC(bp), ptr);

        PUT(HDRP(ptr), PACK(size - asize, 0));
        PUT(FTRP(ptr), PACK(size - asize, 0));
    }
    else 
    {
        /* 无需分割*/
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));

        SET_SUCC(GET_PRED(bp), GET_SUCC(bp));
        SET_PRED(GET_SUCC(bp), GET_PRED(bp));
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    char *p;
    size_t size = GET_SIZE(HDRP(ptr));
    /*这里插入要注意地址顺序*/
    for (p = GET_SUCC(free_list_headp); ; p = GET_SUCC(p))
    {
        if (ptr < (void *)p) 
        {
            /* 先改变分配状态 */
            PUT(HDRP(ptr), PACK(size, 0));
            PUT(FTRP(ptr), PACK(size, 0));

            SET_SUCC(ptr, p);
			SET_PRED(ptr, GET_PRED(p));
            SET_SUCC(GET_PRED(p), ptr);
            SET_PRED(p, ptr);
            break;
        }
    }
    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *old_ptr = ptr;
    void *new_ptr;
	void *next_ptr;
	void *pred;
	void *succ;
	char *p;

	size_t block_size;
    size_t extend_size;
	size_t asize;
	size_t sum_size;

    if (ptr == NULL) /* 直接调用mm_malloc */
    {
        return mm_malloc(size);
    }
    else if (size == 0) /* 直接调用mm_free*/
    {
        mm_free(ptr);
        return NULL;
    }

    /* 调整大小*/ 
    if (size <= 2*DSIZE)
    {
        asize = 3 * DSIZE;
    }
    else
    {
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
    }
    
    block_size = GET_SIZE(HDRP(ptr)); 
    if (asize == block_size)
    {
        return ptr;
    }
    else if (asize < block_size)
    {
        if ((block_size - asize) >= 3 * DSIZE)
        {
            PUT(HDRP(ptr), PACK(asize, 1));
		    PUT(FTRP(ptr), PACK(asize, 1));
            next_ptr = NEXT_BLKP(ptr);
            PUT(HDRP(next_ptr), PACK(block_size - asize, 0));
            PUT(FTRP(next_ptr), PACK(block_size - asize, 0));

            for (p = GET_SUCC(free_list_headp); ; p = GET_SUCC(p))
            {
                if (next_ptr < (void *)p)
                {
                    pred = GET_PRED(p);
					succ = p;
					SET_PRED(next_ptr, pred);
					SET_SUCC(next_ptr, succ);
					SET_SUCC(pred, next_ptr);
					SET_PRED(p, next_ptr);
					break;
                }
            }
        }
        return ptr;
    }
    else /* asize > block_size */
    {
        next_ptr = NEXT_BLKP(ptr);
        sum_size = GET_SIZE(HDRP(next_ptr)) + block_size;/*获得总大小*/

        if (!GET_ALLOC(HDRP(next_ptr)) && (sum_size >= asize)) 
        {
            pred = GET_PRED(next_ptr);
            succ = GET_SUCC(next_ptr);
            if ((sum_size - asize) >= 3*DSIZE) 
            {
                PUT(HDRP(ptr), PACK(asize, 1));
				PUT(FTRP(ptr), PACK(asize, 1));
                next_ptr = NEXT_BLKP(ptr);
				PUT(HDRP(next_ptr), PACK(sum_size - asize, 0));
				PUT(FTRP(next_ptr), PACK(sum_size - asize, 0));

                SET_PRED(next_ptr, pred);
				SET_SUCC(next_ptr, succ);
				SET_SUCC(pred, next_ptr);
				SET_PRED(succ, next_ptr);
            }
            else 
            {
                PUT(HDRP(ptr), PACK(sum_size, 1));
				PUT(FTRP(ptr), PACK(sum_size, 1));
				SET_SUCC(pred, succ);
				SET_PRED(succ, pred);
            }
            return ptr;
        }
        else 
        {
            new_ptr = find_fit(asize);
            if (new_ptr == NULL) 
            {
                extend_size = MAX(asize, CHUNKSIZE);
                if ((new_ptr = extend_heap(extend_size / WSIZE)) == NULL)
                {
                    return NULL;
                }
            }
            place(new_ptr, asize);
            memcpy(new_ptr, old_ptr, block_size - 2*WSIZE);/*复制*/
            mm_free(old_ptr); 
            return new_ptr;
        }
    }
}