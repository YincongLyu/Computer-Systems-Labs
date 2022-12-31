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

#define VERBOSE 0
#ifdef DEBUG
#define VERBOSE 1
#endif

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */  // 对 ALIGNMENT 倍数上取整的计算
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 自定义的宏，有便于操作常量和指针运算 */   
/* Base constants and macros */
#define WSIZE		4		/* 字、脚部或头部的大小（字节） */
#define DSIZE		8		/* 双字大小（字节） */
#define CHUNKSIZE	(1<<12)	/* 扩展堆时的默认大小 */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* 将 size 和 allocated bit 合并为一个字*/
#define PACK(size, alloc)	((size) | (alloc))

#define GET(p)		(*(unsigned int *)(p))         /* 读地址p处的一个字 */
#define PUT(p, val) (*(unsigned int *)(p) = val)   /* 向地址p处写一个字*/

#define GET_PTR_VAL(p)  (*(unsigned long *)(p))    /* 读地址p处的一个指针 */
#define SET_PTR(p, ptr) (*(unsigned long *)(p) = (unsigned long)(ptr))  /* 向地址p处写一个指针 */

/* 读写指向祖先、后继指针的指针 */
#define GET_PRED(p) ((char *)(*(unsigned long *)(p)))
#define GET_SUCC(p) ((char *)(*(unsigned long *)(p + DSIZE)))
#define SET_PRED(p, ptr)	(SET_PTR((char *)(p), ptr))
#define SET_SUCC(p, ptr)	(SET_PTR(((char *)(p)+(DSIZE)), ptr))

#define GET_SIZE(p)		(GET(p) & ~0x7)    //得到地址p处的 size
#define GET_ALLOC(p)	(GET(p) & 0x1)      //得到地址p处的 allocated bit

/* 获得头部脚部地址 */
#define HDRP(bp)	((char* )(bp) - WSIZE)
#define FTRP(bp)	((char* )(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* 计算后块、前块地址 */
#define NEXT_BLKP(bp)	((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)	((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Free list pointer */
static char *free_list_headp;   //指向序言块
static char *free_list_tailp;   //空闲链表的头结点，存放在堆区开关的对齐块中
/* private functions */
static void *extend_heap(size_t words);   //拓展堆块
static void *coalesce(void *ptr);         //合并空闲块
static void *find_fit(size_t asize);      //寻找空闲块
static void place(void *bp, size_t asize);//分割空闲块



/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	/* Create the initial empty heap(40 bytes) */
	if ((free_list_headp = mem_sbrk(4*WSIZE+3*DSIZE)) == (void *)-1) {
		return -1;
	}
	PUT(free_list_headp, PACK(0, 0));					/* Padding - for alignment 8 bytes */
	PUT(free_list_headp+WSIZE, PACK(24, 1));			/* Prologue header */
	free_list_headp += DSIZE;							/* free list head ptr */
	free_list_tailp = NEXT_BLKP(free_list_headp);		/* free list tail ptr */
	SET_PRED(free_list_headp, NULL);					/* Prologue pred */
	SET_SUCC(free_list_headp, free_list_tailp);			/* Prologue succ */
	PUT(free_list_headp+(2*DSIZE), PACK(24, 1));		/* Prologue footer */

	PUT(HDRP(free_list_tailp), PACK(0, 1));				/* Epilogue header */
	SET_PRED(free_list_tailp, free_list_headp);			/* Epilogue pred */
	PUT(free_list_tailp+DSIZE, PACK(0, 0));				/* Padding - for alignment 8 bytes */
	/* Extend the empty heap with a free block of CHUNKSIZE bytes */
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL) {
		return -1;
	}
    
    return 0;
}

static void *extend_heap(size_t words)
{
	char *bp;
	char *ptr;
	size_t size;
	
	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;
	
	/* Initialize free block header/footer and the epilogue header */
	ptr = free_list_tailp;
	PUT(HDRP(ptr), PACK(size, 0));		/* Free block header */
	PUT(FTRP(ptr), PACK(size, 0));		/* Free block footer */
	free_list_tailp = NEXT_BLKP(ptr);	/* Update free list tailp */
	SET_SUCC(ptr, free_list_tailp);		/* Update free list */

	/* New epilogue block */
	PUT(HDRP(free_list_tailp), PACK(0, 1));
	SET_PRED(free_list_tailp, ptr);
	PUT(free_list_tailp+DSIZE, PACK(0, 0));
	/* Coalesce if the previous block was free */
	return coalesce(ptr);
	
}


void mm_free(void *ptr)
{
	char *p;
	size_t size = GET_SIZE(HDRP(ptr));

	/* Maintain free list in address order */
	for (p = GET_SUCC(free_list_headp); ; p = GET_SUCC(p)) {
		if (ptr < (void *)p) {		/* Find a suitable position- the first free block after this block. */
			PUT(HDRP(ptr), PACK(size, 0));
			PUT(FTRP(ptr), PACK(size, 0));
			SET_SUCC(ptr, p);
			SET_PRED(ptr, GET_PRED(p));
			/* Change previous and next free block */
			SET_SUCC(GET_PRED(p), ptr);
			SET_PRED(p, ptr);
			break;
		}
	}

	coalesce(ptr);
}

/* coalesce -- Coalesce free block */
static void *coalesce(void *ptr)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
	size_t size = GET_SIZE(HDRP(ptr));
	
	void *prevptr = PREV_BLKP(ptr);
	void *nextptr = NEXT_BLKP(ptr);
	if (prev_alloc && next_alloc) {				/* Case 1 - Nothing to do */
		return ptr;
	} else if (prev_alloc && !next_alloc) {		/* Case 2 */
		size += GET_SIZE(HDRP(nextptr));
		SET_SUCC(ptr, GET_SUCC(nextptr));
		SET_PRED(GET_SUCC(nextptr), ptr);
		PUT(HDRP(ptr), PACK(size, 0));
		PUT(FTRP(nextptr), PACK(size, 0));
	} else if (!prev_alloc && next_alloc) {		/* Case 3 */
		size += GET_SIZE(FTRP(prevptr));
		SET_SUCC(prevptr, GET_SUCC(ptr));
		SET_PRED(GET_SUCC(ptr), prevptr);
		PUT(HDRP(prevptr), PACK(size, 0));
		PUT(FTRP(ptr), PACK(size, 0));
		ptr = prevptr;
	} else if (!prev_alloc && !next_alloc) {	/* Case 4 */
		size += GET_SIZE(HDRP(prevptr)) 
			+ GET_SIZE(FTRP(nextptr));
		SET_SUCC(prevptr, GET_SUCC(nextptr));
		SET_PRED(GET_SUCC(nextptr), prevptr);
		PUT(HDRP(prevptr), PACK(size, 0));
		PUT(FTRP(nextptr), PACK(size, 0));
		ptr = prevptr;
	}
    
	return ptr;
}

void *mm_malloc(size_t size)
{
    size_t asize;		/* Adjusted block size */
    size_t extendsize;	/* Amount to extend heap if no fit */
   	char *bp;

   	/* Ignore spurious requests */
   	if (size == 0)
   		return NULL;
   	
   	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= 2*DSIZE) {
		asize = 3*DSIZE;
	} else {
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
	}

   	/* Search the free list for a fit */
   	if ((bp = find_fit(asize)) != NULL) {
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

/* find_fit -- find the first fit free block in free list. */
static void *find_fit(size_t asize)
{
	void* p;

	for (p = GET_SUCC(free_list_headp); p != free_list_tailp; p = GET_SUCC(p)) {
		if (asize <= GET_SIZE(HDRP(p)))
			return p;
	}

	return NULL;
}


/* place when remaining part size is greater than 6 word, divide it. */
static void place(void *bp, size_t asize)
{
	size_t size = GET_SIZE(HDRP(bp));
	void *ptr;

	if (size - asize >= 3*DSIZE) {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		ptr = NEXT_BLKP(bp);
		/* Set next block - succ and pred */
		SET_SUCC(ptr, GET_SUCC(bp));
		SET_PRED(ptr, GET_PRED(bp));
		/* Change the previous and next free block state. */
		SET_SUCC(GET_PRED(ptr), ptr);
		SET_PRED(GET_SUCC(ptr), ptr);
		
		PUT(HDRP(ptr), PACK(size-asize, 0));
		PUT(FTRP(ptr), PACK(size-asize, 0));
		
	} else {
		PUT(HDRP(bp), PACK(size, 1));
		PUT(FTRP(bp), PACK(size, 1));

		/* Change the previous and next free block state */
		SET_SUCC(GET_PRED(bp), GET_SUCC(bp));
		SET_PRED(GET_SUCC(bp), GET_PRED(bp));
	}
}

void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
	void *nextptr;
	void *pred;
	void *succ;
	char *p;

	size_t blockSize;
    size_t extendsize;
	size_t asize;	/* Adjusted block size */
	size_t sizesum;

	if (ptr == NULL) { 			/* If ptr == NULL, call mm_alloc(size) */
		return mm_malloc(size);
	} else if (size == 0) { 	/* If size == 0, call mm_free(size) */
		mm_free(ptr);
		return NULL;
	}

	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= 2*DSIZE) {
		asize = 3*DSIZE;
	} else {
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
	}

	blockSize = GET_SIZE(HDRP(ptr));

	
	if (asize == blockSize) {						/* Case 1: asize == block size, nothing to do.  */
		return ptr;
	} else if (asize < blockSize) {					/* Case 2: asize < blockSize, */
		if (blockSize-asize >= 3*DSIZE) {	
			/* Change this block header and footer */
			PUT(HDRP(ptr), PACK(asize, 1));
			PUT(FTRP(ptr), PACK(asize, 1));
			/* Change next block header and footer */
			nextptr = NEXT_BLKP(ptr);
			PUT(HDRP(nextptr), PACK(blockSize-asize, 0));
			PUT(FTRP(nextptr), PACK(blockSize-asize, 0));
			/* Find a suitable place in free list , insert into free list. */
			for (p = GET_SUCC(free_list_headp); ; p = GET_SUCC(p)) {
				if (nextptr < (void *)p) {
					pred = GET_PRED(p);
					succ = p;
					SET_PRED(nextptr, pred);
					SET_SUCC(nextptr, succ);
					SET_SUCC(pred, nextptr);
					SET_PRED(p, nextptr);
					break;
				}
			}
		} 
		/*  OR remaining block size is less than 24 bytes, cannot divide, nothing to do */
		return ptr;
	} else {										/* Case 3: asize > blockSize */
		/* Check next block */
		nextptr = NEXT_BLKP(ptr);
		sizesum = GET_SIZE(HDRP(nextptr))+blockSize;	/* Sum of this and next block size */
		if (!GET_ALLOC(HDRP(nextptr)) && sizesum >= asize) {	/* Next block is free and size is big enough. */
			/* Change free list */
			pred = GET_PRED(nextptr);
			succ = GET_SUCC(nextptr);
			if (sizesum-asize >= 3*DSIZE) {
				PUT(HDRP(ptr), PACK(asize, 1));
				PUT(FTRP(ptr), PACK(asize, 1));
				/* Set next block header, footer and pred, succ and insert it into free list. */
				nextptr = NEXT_BLKP(ptr);
				PUT(HDRP(nextptr), PACK(sizesum-asize, 0));
				PUT(FTRP(nextptr), PACK(sizesum-asize, 0));
				/* Insert into free list. */
				SET_PRED(nextptr, pred);
				SET_SUCC(nextptr, succ);
				SET_SUCC(pred, nextptr);
				SET_PRED(succ, nextptr);
			} else {
				PUT(HDRP(ptr), PACK(sizesum, 1));
				PUT(FTRP(ptr), PACK(sizesum, 1));
				/* delete next free block out of free list. */
				SET_SUCC(pred, succ);
				SET_PRED(succ, pred);
			}
			return ptr;
		} else {		/* Next block is allocated or size is not enough. */
			newptr = find_fit(asize);
			if (newptr == NULL) {
				extendsize = MAX(asize, CHUNKSIZE);
				if ((newptr = extend_heap(extendsize/WSIZE)) == NULL) {	/* Can not find a fit block, it must allocate memory from heap. */
					return NULL;
				}
			}
			place(newptr, asize);
			memcpy(newptr, oldptr, blockSize-2*WSIZE);
			mm_free(oldptr);
			return newptr;
		}
	}

}
