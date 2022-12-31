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
    "sc",
    /* First member's full name */
    "Liu Luying",
    /* First member's email address */
    "10215501428@stu.ecnu.edu.cn",
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
/* Base constants and macros */
#define WSIZE		4		/* Word and header/footer size (bytes) */
#define DSIZE		8		/* Double word size (bytes) */
#define CHUNKSIZE	(1<<12)	/* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word*/
#define PACK(size, alloc)	((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)		(*(unsigned int *)(p))
#define PUT(p, val)	(*(unsigned int *)(p) = val)

/* Get and Set pointer value by ptr at address p */
#define GET_PTR_VAL(p)	(*(unsigned long *)(p))
#define SET_PTR(p, ptr)	(*(unsigned long *)(p) = (unsigned long)(ptr))

/* Read and write pred and succ pointer at address p */
#define GET_PRED(p)	((char *)(*(unsigned long *)(p)))
#define GET_SUCC(p)	((char *)(*(unsigned long *)(p + DSIZE)))
#define SET_PRED(p, ptr)	(SET_PTR((char *)(p), ptr))
#define SET_SUCC(p, ptr)	(SET_PTR(((char *)(p)+(DSIZE)), ptr))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)		(GET(p) & ~0x7)
#define GET_ALLOC(p)	(GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)	((char* )(bp) - WSIZE)
#define FTRP(bp)	((char* )(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)	((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)	((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Free list pointer */
static char *free_list_headp;
static char *free_list_tailp;

static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);

int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void* bp);
void *mm_realloc(void* ptr, size_t size);


/*
 * coalesce - 合并空闲块，同时维护链表
 * bp位置是一个新的节点，已设置好头部与脚部，但未设置PREV与NEXT
 */
static void *coalesce(void *ptr)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
	size_t size = GET_SIZE(HDRP(ptr));
	
	void *prev_ptr = PREV_BLKP(ptr);
	void *next_ptr = NEXT_BLKP(ptr);
    
	if (prev_alloc && next_alloc) {
        //case1
		return ptr;
	} else if (prev_alloc && !next_alloc) {		
        //case2
		size += GET_SIZE(HDRP(next_ptr));
		SET_SUCC(ptr, GET_SUCC(next_ptr));
		SET_PRED(GET_SUCC(next_ptr), ptr);
		PUT(HDRP(ptr), PACK(size, 0));
		PUT(FTRP(next_ptr), PACK(size, 0));
	} else if (!prev_alloc && next_alloc) {		
        //case3
		size += GET_SIZE(FTRP(prev_ptr));
		SET_SUCC(prev_ptr, GET_SUCC(ptr));
		SET_PRED(GET_SUCC(ptr), prev_ptr);
		PUT(HDRP(prev_ptr), PACK(size, 0));
		PUT(FTRP(ptr), PACK(size, 0));
		ptr = prev_ptr;
	} else if (!prev_alloc && !next_alloc) {	
        //case4
		size += GET_SIZE(HDRP(prev_ptr)) 
			+ GET_SIZE(FTRP(next_ptr));
		SET_SUCC(prev_ptr, GET_SUCC(next_ptr));
		SET_PRED(GET_SUCC(next_ptr), prev_ptr);
		PUT(HDRP(prev_ptr), PACK(size, 0));
		PUT(FTRP(next_ptr), PACK(size, 0));
		ptr = prev_ptr;
	}
    
	return ptr;
}

/*
 * extend_heap - 无空闲块时，返回块指针
 */
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

/*
 * find_fit
 */
static void *find_fit(size_t asize)
{
	void* p;

	for (p = GET_SUCC(free_list_headp); p != free_list_tailp; p = GET_SUCC(p)) {
		if (asize <= GET_SIZE(HDRP(p)))
			return p;
	}

	return NULL;
}

/*
 * place - 放置空闲块，剩余部分超出最小块大小时分割
 */
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

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
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

/*
 * mm_free - Freeing a block does nothing.
 */
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


/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
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
	size_t asize;
	size_t sizesum;

	if (ptr == NULL) { 	
		return mm_malloc(size);
	} else if (size == 0) { 
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

	
	if (asize == blockSize) {						
        //case 1
		return ptr;
	} else if (asize < blockSize) {					
        //case2
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
		return ptr;
	} 
    else {										
        //case3
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
				/* insert */
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
				if ((newptr = extend_heap(extendsize/WSIZE)) == NULL) {	
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