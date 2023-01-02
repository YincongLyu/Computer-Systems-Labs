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
    "lzx",
    /* First member's email address */
    "3027381476@qq.com",
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

#define WSIZE         4         /* 字的大小为 4 字节 */
#define DSIZE         8         /* 双字大小 */
#define CHUNKSIZE    (1 << 10)  /* 堆扩展时默认大小 */

#define MAX(x, y)    ((x) > (y) ? (x) : (y))

#define PACK(size, alloc)    ((size) | (alloc)) /* 详见GET_SIZE和GET_ALLOC */

/* Read and write a word at address p.
 * 一个字 4 字节所以用int
 */
#define GET(p)         (*(unsigned int *)(p))
#define PUT(p, val)    (*(unsigned int *)(p) = (val))

/* Get and Set pointer value by ptr at address p.
 * 64 位指针为 8 字节，所以用long，下同
 */
#define GET_PTR(p)         (*(unsigned long *)(p))
#define SET_PTR(p, ptr)    (*(unsigned long *)(p) = (unsigned long)(ptr))

/* Read and write pred and succ pointer at address p */
#define GET_PRED(p)         ((char *)(*(unsigned long *)(p)))
#define GET_SUCC(p)         ((char *)(*(unsigned long *)(p + DSIZE))) /* PRED指针大小为 8 */
#define SET_PRED(p, ptr)    (SET_PTR((char *)(p), ptr))
#define SET_SUCC(p, ptr)    (SET_PTR(((char *)(p) + DSIZE), ptr))

/* 头部最低三位用来记录状态，最后一位 0 或 1，表示是否已分配
 * 剩下记录块大小
 */
#define GET_SIZE(p)     (GET(p) & (~0x7))
#define GET_ALLOC(p)    (GET(p) & 0x1)

#define HDRP(bp)    ((char *)(bp) - WSIZE) /* bp指向第一个有效载荷(payload)字节, 前一个字节为头部 */
#define FTRP(bp)    ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) /* block的大小包括headr和footr */

/* 都是指向第一个payload字节 */
#define NEXT_BLKP(bp)    ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)    ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) /* 获得上一个块的脚部 */

static char *free_list_headp; /* 显式空闲链表头 */
static char *free_list_tailp; /* 显式空闲链表尾 */

static void *coalesce(void *ptr); /* 合并 */
static void *extend_heap(size_t words); /* 扩展堆 */
static void *find_fit(size_t asize); /* 返回第一个合适的空闲块 */
static void place(void *bp, size_t asize); /* 放（包括分割） */

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((free_list_headp = mem_sbrk(5 * DSIZE)) == (void *)-1)
    {
        return -1;
    }

    PUT(free_list_headp, PACK(0, 0)); /* 为了对齐 8 */
    
    /* free_list_headp的block有 24 字节，
     * 在free_list_headp += DSIZE之后，
     * free_list_headp + WSIZE相当于free_list_headp的header
     * 序言块
     */
    PUT(free_list_headp + WSIZE, PACK(24, 1));
    //printf("%p", (char *)free_list_headp);
    free_list_headp += DSIZE; /* 往后挪俩 */
    //printf("%p", (char *)free_list_headp);
    free_list_tailp = NEXT_BLKP(free_list_headp);
    //printf("%p", (char *)free_list_tailp);
    SET_PRED(free_list_headp, NULL);
    SET_SUCC(free_list_headp, free_list_tailp);
    PUT(free_list_headp + (2 * DSIZE), PACK(24, 1)); /* free_list_headp的footer */
    
    /* 结尾块 */
    PUT(HDRP(free_list_tailp), PACK(0, 1));
    SET_PRED(free_list_tailp, free_list_headp);
    // PUT(free_list_tailp + DSIZE, PACK(0, 0)); /* 使得结尾块大小为 8 的倍数（12 -> 16） */

    /* 初始化扩展堆 */
    if (extend_heap((7*DSIZE) / WSIZE) == NULL)
    {
        return -1;
    }
    return 0;
}

/* 扩展堆 */
static void *extend_heap(size_t words)
{
    //char *bp;
    char *ptr;
    size_t size;

    size = (words % 2) ? (words + 1)*WSIZE : words * WSIZE; /* 使得size为 8 的倍数 */
    if ((long)(mem_sbrk(size)) == -1) /* extend the heap */
    {
        return NULL;
    }
    ptr = free_list_tailp;

    /* ptr指向扩展后新区域的第一个payload字节
     * 设置状态
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
    /* 获得前后block的分配状态 */
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));

    size_t size = GET_SIZE(HDRP(ptr));

    void *prev_ptr = PREV_BLKP(ptr);
    void *next_ptr = NEXT_BLKP(ptr);

    if (prev_alloc && next_alloc) /* 前面的块和后面的块都是已分配的 */
    {
        return ptr;
    }
    else if (prev_alloc && !next_alloc) /* 前面的块是已分配的，后面的块是空闲的 */
    {
        size += GET_SIZE(HDRP(next_ptr));

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

    /* 调整大小 */
    if (size == 0) /* 听君一席话，如听一席话 */
    {
        return NULL;
    }
    if (size <= 2*DSIZE) /* 最小块overhead一共有 24 字节 */
    {
        asize = 3 * DSIZE;
    }
    else /* 向上舍入到最接近 8 的整数倍 */
    {
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
    }

    /* 进行分配 */
    if ((bp = find_fit(asize)) != NULL) /* 找到合适块 */
    {
        place(bp, asize);
        return bp;
    }
    /* 未找到合适块，扩展堆 */
    extend_size = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extend_size / WSIZE)) == NULL)
    {
        return NULL;
    }
    place(bp, asize);
    return bp;
}

/* 找合适块，返回第一个找到的块 */
static void *find_fit(size_t asize)
{
    void *p;

    /* 每次都从头遍历 */
    for (p = GET_SUCC(free_list_headp); p != free_list_tailp; p = GET_SUCC(p))
    {
        if (asize <= GET_SIZE(HDRP(p)))
        {
            return p;
        }
    }
    return NULL; /* 没找到 */
}

/* 将分配的指针和大小放入块中，剩余大于 24 字节需要分割块 */
static void place(void *bp, size_t asize)
{
    size_t size = GET_SIZE(HDRP(bp));
    void *ptr;

    if ((size - asize) >= 3*DSIZE) /* 剩余大于 24 字节要分割 */
    {
        PUT(HDRP(bp), PACK(asize, 1)); /* 分配恰好的尺寸 */
        PUT(FTRP(bp), PACK(asize, 1));

        /* 接下来进行分割
         * 前面bp的size已经改为asize，所以ptr是bp被分割的剩余部分，
         * 继承bp的前驱后继，有四个状态要改
         */
        ptr = NEXT_BLKP(bp);
        SET_SUCC(ptr, GET_SUCC(bp));
        SET_PRED(ptr, GET_PRED(bp));
        SET_SUCC(GET_PRED(bp), ptr);
        SET_PRED(GET_SUCC(bp), ptr);

        /* 修改ptr的大小和分配状态 */
        PUT(HDRP(ptr), PACK(size - asize, 0));
        PUT(FTRP(ptr), PACK(size - asize, 0));
    }
    else /* 剩余小于 24 字节 */
    {
        /* 无需分割，整个bp都分配出去 */
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

    /* 维持空闲链表的地址顺序
     * 头 -> 尾 : (地址) 小 -> 大
     */
    for (p = GET_SUCC(free_list_headp); ; p = GET_SUCC(p))
    {
        if (ptr < (void *)p) /* 将ptr插到p前面 */
        {
            /* 先改变ptr的分配状态 */
            PUT(HDRP(ptr), PACK(size, 0));
            PUT(FTRP(ptr), PACK(size, 0));
            /* 设置前驱后继 */
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

    if (ptr == NULL) /* 原先没有指针已分配，直接分配新的 */
    {
        return mm_malloc(size);
    }
    else if (size == 0) /* 释放 */
    {
        mm_free(ptr);
        return NULL;
    }

    /* 调整大小 (同mm_malloc) */ 
    if (size <= 2*DSIZE)
    {
        asize = 3 * DSIZE;
    }
    else
    {
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
    }
    
    block_size = GET_SIZE(HDRP(ptr)); /* 已排除 (ptr == NULL) 的情况 */

    if (asize == block_size) /* 重新分配的大小与原来相同，直接返回 */
    {
        return ptr;
    }
    else if (asize < block_size)
    {
        /* 同place */
        if ((block_size - asize) >= 3 * DSIZE)
        {
            PUT(HDRP(ptr), PACK(asize, 1));
		    PUT(FTRP(ptr), PACK(asize, 1));
            next_ptr = NEXT_BLKP(ptr);
            PUT(HDRP(next_ptr), PACK(block_size - asize, 0));
            PUT(FTRP(next_ptr), PACK(block_size - asize, 0));

            /* 同free，按地址顺序找到合适的位置*/
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
        return ptr; /* 剩余块大于 24 的处理后返回；小于 24 的直接返回，不用处理 */
    }
    else /* asize > block_size */
    {
        next_ptr = NEXT_BLKP(ptr);
        sum_size = GET_SIZE(HDRP(next_ptr)) + block_size;

        if (!GET_ALLOC(HDRP(next_ptr)) && (sum_size >= asize)) /* 下一个块未分配且总大小足够 */
        {
            pred = GET_PRED(next_ptr);
            succ = GET_SUCC(next_ptr);
            if ((sum_size - asize) >= 3*DSIZE) /* 分割 */
            {
                PUT(HDRP(ptr), PACK(asize, 1));
				PUT(FTRP(ptr), PACK(asize, 1));
                next_ptr = NEXT_BLKP(ptr);
				PUT(HDRP(next_ptr), PACK(sum_size - asize, 0));
				PUT(FTRP(next_ptr), PACK(sum_size - asize, 0));

                /* 这里是空闲块分割，空闲链表是有序的，不用再比较地址 */
                SET_PRED(next_ptr, pred);
				SET_SUCC(next_ptr, succ);
				SET_SUCC(pred, next_ptr);
				SET_PRED(succ, next_ptr);
            }
            else /* 不用分割 */
            {
                PUT(HDRP(ptr), PACK(sum_size, 1));
				PUT(FTRP(ptr), PACK(sum_size, 1));
				SET_SUCC(pred, succ);
				SET_PRED(succ, pred);
            }
            return ptr;
        }
        else /* 下一个块已分配或大小不够 */
        {
            new_ptr = find_fit(asize);
            if (new_ptr == NULL) /* 扩展堆 */
            {
                extend_size = MAX(asize, CHUNKSIZE);
                if ((new_ptr = extend_heap(extend_size / WSIZE)) == NULL)
                {
                    return NULL;
                }
            }
            place(new_ptr, asize);
            /* memcpy最后一个参数是复制的字节数，新旧指针都是指向第一个payload字节
             * 头部脚部在extend_heap里设置
             * 所以减去 2 * WSIZE，复制payload部分
             */
            memcpy(new_ptr, old_ptr, block_size - 2*WSIZE);
            mm_free(old_ptr); /* 这里直接扩展堆，旧的复制过去，所以旧的块直接删 */
            return new_ptr;
        }
    }
}