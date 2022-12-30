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
#define SingleWordSize 4
#define InitSize (1<<12) 
#define MinBlockSize 16
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
/* combine size and allocation status */
#define HeadAndFoot(size, alloc) ((size) | (alloc)) 
/* read or write the value of a pointer */
#define GetPointer(p) (*(unsigned int *)(p)) 
#define WritePointer(p, val) (*(unsigned int *)(p) = (val)) 
/* get the size or allocation status of a block */
#define GetSize(p) (GetPointer(p) & ~0x7) 
#define IfAllocated(p) (GetPointer(p) & 0x1) 
/* get the head or foot of a block */
#define FindHead(bp) ((char*)(bp) - SingleWordSize) 
#define FindFoot(bp) ((char*)(bp) + GetSize(FindHead(bp)) - ALIGNMENT) 
/* find the last or next block of a block */
#define NextBlock(bp) ((char*)(bp) + GetSize(FindHead(bp))) 
#define LastBlock(bp) ((char*)(bp) - GetSize((char*)(bp)-ALIGNMENT))
static char* heap_listp;
static char* pre_listp;
static void* extend_heap(size_t words);
static void* coalesce(void *bp);
static void* find_fit(size_t asize);
static void split_block(void* bp, size_t asize);
static void place(void* bp, size_t asize);
int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *bp);
void *mm_realloc(void *ptr, size_t size);
/*
 * * ExtendHeap - extend the heap area.
 */
static void* extend_heap(size_t words)
{
    char* bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words+(words%2))*SingleWordSize;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    WritePointer(FindHead(bp), HeadAndFoot(size, 0)); //free block header
    WritePointer(FindFoot(bp), HeadAndFoot(size, 0)); //free block footer
    WritePointer(FindHead(NextBlock(bp)), HeadAndFoot(0, 1)); // new epilogue header

    return coalesce(bp); //coalesce if the previous block was free
}
static void* coalesce(void *bp)
{
        size_t prev_alloc = IfAllocated(FindFoot(LastBlock(bp)));
        size_t next_alloc = IfAllocated(FindHead(NextBlock(bp)));
        size_t size = GetSize(FindHead(bp));
        if (prev_alloc && next_alloc)
	{
		pre_listp = bp;
                return bp;
	}
	else if(prev_alloc && !next_alloc)
        {
                size=size+GetSize(FindHead(NextBlock(bp)));
                WritePointer(FindHead(bp),HeadAndFoot(size,0));
                WritePointer(FindFoot(bp),HeadAndFoot(size,0));
        }
        else if (!prev_alloc && next_alloc)
        {
                size=size+GetSize(FindHead(LastBlock(bp)));
                WritePointer(FindFoot(bp), HeadAndFoot(size, 0));
                WritePointer(FindHead(LastBlock(bp)), HeadAndFoot(size, 0));
                bp = LastBlock(bp);
        }
        else
        {
                size=size+GetSize(FindHead(LastBlock(bp)))+GetSize(FindFoot(NextBlock(bp)));
                WritePointer(FindHead(LastBlock(bp)),HeadAndFoot(size,0));
                WritePointer(FindHead(NextBlock(bp)),HeadAndFoot(size,0));
                bp = LastBlock(bp);
        }
	pre_listp = bp;
        return bp;
}
/* 
 * FirstFit - find the first block that meets the requirements.
 */
static void* find_fit(size_t newsize)
{
	for (char* bp = pre_listp;GetSize(FindHead(bp))>0;bp = NextBlock(bp))
        {
                if(GetSize(FindHead(bp))>=newsize && !IfAllocated(FindHead(bp)))
                {
                        return bp;
                }
        }
	for (char* bp = heap_listp;GetSize(FindHead(bp))>0;bp = NextBlock(bp))
	{
		if(GetSize(FindHead(bp))>=newsize && !IfAllocated(FindHead(bp)))
		{
			return bp;
		}
	}
	return NULL;
}

/* 
 * place and split a free block.
 */
static void split_block(void *bp,size_t newsize)
{
	size_t size = GetSize(FindHead(bp));
	if((size-newsize)>=MinBlockSize)
	{
		WritePointer(FindHead(bp), HeadAndFoot(newsize, 1));
		WritePointer(FindFoot(bp), HeadAndFoot(newsize, 1));
		pre_listp = bp;
		bp = NextBlock(bp);
		WritePointer(FindHead(bp), HeadAndFoot(size-newsize, 0));
		WritePointer(FindFoot(bp), HeadAndFoot(size-newsize, 0));
	}
}

static void place(void *bp,size_t newsize)
{
	size_t size = GetSize(FindHead(bp));
	WritePointer(FindHead(bp), HeadAndFoot(size, 1));
	WritePointer(FindFoot(bp), HeadAndFoot(size, 1));
	split_block(bp,newsize);
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if((heap_listp = mem_sbrk(4*SingleWordSize)) == (void*)-1)
    {
	    return -1;
    }
    WritePointer(heap_listp,0);
    WritePointer(heap_listp+(1*SingleWordSize),HeadAndFoot(ALIGNMENT,1));
    WritePointer(heap_listp+(2*SingleWordSize),HeadAndFoot(ALIGNMENT,1));
    WritePointer(heap_listp+(3*SingleWordSize),HeadAndFoot(0,1));
    heap_listp = heap_listp+ALIGNMENT;
    pre_listp=heap_listp;
    if(extend_heap(InitSize / SingleWordSize)==NULL)
    {
	    return -1;
    }
    else
    {
	    return 0;
    }
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t newsize;
    size_t extend;
    char *p;
    if (size==0)
    {
	    return NULL;
    }
    if(size<=ALIGNMENT)
    {
	    newsize = 2*ALIGNMENT;
    }
    else
    {
	    newsize = ALIGNMENT * ((size + (ALIGNMENT) + (ALIGNMENT - 1)) / ALIGNMENT);
    }
    if((p = find_fit(newsize))!=NULL)
    {
	    place(p,newsize);
	    return p;
    }
    extend = newsize;
    if(extend<InitSize)
    {
	    extend = InitSize;
    }
    if((p = extend_heap(extend / SingleWordSize))!=NULL)
    {
	 place(p,newsize);
    }
    return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
	size_t size = GetSize(FindHead(ptr));
	WritePointer(FindHead(ptr),HeadAndFoot(size,0));
	WritePointer(FindFoot(ptr),HeadAndFoot(size,0));
	coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
