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


#define LISTMAX 16

/* Basic constants and functions defined by macros */
#define SSIZE 4 /* Single word/header/footer size (bytes) */
#define DSIZE 8 /* Double word size (bytes) */
#define INITCHUNKSIZE (1<<6) /* Extend the empty heap by this amount (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */

/* Take the maximum/minimum value between x and y */
#define MAX(x, y) ((x) > (y)? (x) : (y))
#define MIN(x, y) ((x) < (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - SSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - SSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Predecessor and successor in explict free linkedlist */
#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + SSIZE)
#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_PTR(ptr)))
#define SET_PTR(p, ptr) (*(unsigned int *)(p) = (unsigned int)(ptr)) /* put the address of ptr in p */

static void *extend_heap(size_t words);
static size_t get_asize(size_t size);
static void insert_node(void *bp, size_t size);
static void delete_node(void *bp);
static void *place(void *bp, size_t asize);
static void *coalesce(void *bp);
static void *realloc_coalesce(void *bp,size_t newSIZE,int *isNextFree);
static void realloc_place(void *bp,size_t asize);

void *seg_free_lists[LISTMAX];
char *heap_listp;

static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * SSIZE : words * SSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    
    PUT(HDRP(bp), PACK(size, 0)); /* Free block header */
    PUT(FTRP(bp), PACK(size, 0)); /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free include insert*/
    return coalesce(bp);
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    int i;
    /* initialize seg_free_lists */
    for (i = 0; i < LISTMAX; i++) {
        seg_free_lists[i] = NULL;
    }
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*SSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0); /* Alignment padding */
    PUT(heap_listp + (1*SSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (2*SSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (3*SSIZE), PACK(0, 1)); /* Epilogue header */
    heap_listp += (2*SSIZE);

    /* Extend the empty heap with a free block of INITCHUNKSIZE bytes */
    if (extend_heap(INITCHUNKSIZE/SSIZE) == NULL)
        return -1;
    return 0;
}

static size_t get_asize(size_t size) 
{
    size_t asize;
    if(size <= DSIZE){
        asize = 2*(DSIZE);
    }
    else{
        asize = ALIGN(size + DSIZE);
    }
    return asize;
}

static void insert_node(void *bp, size_t size)
{
    int tar = 0;
    size_t j;
    for (j = size; (j > 1) && (tar < LISTMAX - 1); j >>= 1) { /* find the suitable list */
        tar++;
    }
    char *i = seg_free_lists[tar];
    char *pre = NULL;
    while ((i != NULL) && (size > GET_SIZE(HDRP(i)))) { /* find the first node with at least size (bytes) */
        pre = i;
        i = SUCC(i);
    }
    if (i == NULL && pre == NULL) { /* Case 1: empty list */
        seg_free_lists[tar] = bp;
        SET_PTR(PRED_PTR(bp), NULL);
        SET_PTR(SUCC_PTR(bp), NULL);
    } else if (i == NULL && pre != NULL) { /* Case 2: add at the last */
        SET_PTR(PRED_PTR(bp), pre);
        SET_PTR(SUCC_PTR(bp), NULL);
        SET_PTR(SUCC_PTR(pre), bp);
    } else if (pre == NULL) { /* Case 3: add at the first */
        seg_free_lists[tar] = bp;
        SET_PTR(PRED_PTR(i), bp);
        SET_PTR(SUCC_PTR(bp), i);
        SET_PTR(PRED_PTR(bp), NULL);
    } else { /* Case 4: add in the middle */
        SET_PTR(PRED_PTR(bp), pre);
        SET_PTR(SUCC_PTR(bp), i);
        SET_PTR(PRED_PTR(i), bp);
        SET_PTR(SUCC_PTR(pre), bp);
    }
}

static void delete_node(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp)), j;
    int tar = 0;
    for (j = size; (j > 1) && (tar < LISTMAX - 1); j >>= 1) { /* find the suitable list */
        tar++;
    }
    if (PRED(bp) == NULL) { /* Case 1: delete the only */
        seg_free_lists[tar] = SUCC(bp);
        if (SUCC(bp) != NULL) /* Case 2: delete the first */
            SET_PTR(PRED_PTR(SUCC(bp)), NULL);
    }
    else if (SUCC(bp) == NULL) { /* Case 3: delete the last */
        SET_PTR(SUCC_PTR(PRED(bp)), NULL);
    }
    else {  /* Case 4: delete in the middle */
        SET_PTR(SUCC_PTR(PRED(bp)), SUCC(bp));
        SET_PTR(PRED_PTR(SUCC(bp)), PRED(bp));
    }
}

static void *place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    delete_node(bp);
    if ((csize - asize) >= (2*DSIZE)) {
        if (asize >= 96) {
           PUT(HDRP(bp),PACK(csize - asize,0));
           PUT(FTRP(bp),PACK(csize - asize,0));
           PUT(HDRP(NEXT_BLKP(bp)),PACK(asize,1));
           PUT(FTRP(NEXT_BLKP(bp)),PACK(asize,1));
           insert_node(bp,csize - asize);
           return NEXT_BLKP(bp);
        }
        else {
            PUT(HDRP(bp),PACK(asize,1));
            PUT(FTRP(bp),PACK(asize,1));
            PUT(HDRP(NEXT_BLKP(bp)),PACK(csize - asize,0));
            PUT(FTRP(NEXT_BLKP(bp)),PACK(csize - asize,0));
            insert_node(NEXT_BLKP(bp),csize - asize);
        }
    }
    else {
        PUT(HDRP(bp),PACK(csize,1));
        PUT(FTRP(bp),PACK(csize,1));
    }
    return bp;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize, search_size; /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp = NULL;
    
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;
    
    /* Adjust block size to include overhead and alignment reqs */
    asize = get_asize(size);
    search_size = asize;
    int target;
    for (target = 0; target < LISTMAX; target++, search_size >>= 1) { /* Find target seg_free_list */
        if ((search_size > 1) || (seg_free_lists[target] == NULL))
            continue;
        char *i = seg_free_lists[target];
        for(;i != NULL;i = SUCC(i))
        {
            if (GET_SIZE(HDRP(i)) < asize)
                continue;
            bp = i;
            break;
        }
        if (bp != NULL) break;
    }
    if (bp == NULL) { /* If no fit is found, get more memory and place the block */
        extendsize = MAX(asize,CHUNKSIZE);
        if ((bp = extend_heap(extendsize/SSIZE)) == NULL)
            return NULL;
    }
    bp = place(bp, asize);
    return bp;
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    
    if (prev_alloc && !next_alloc) { /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        delete_node(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size,0));
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        delete_node(PREV_BLKP(bp));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    else if (!prev_alloc && !next_alloc) { /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
        GET_SIZE(FTRP(NEXT_BLKP(bp)));
        delete_node(PREV_BLKP(bp));
        delete_node(NEXT_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    insert_node(bp,size); /* Case 1 */
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

static void *realloc_coalesce(void *bp,size_t newSIZE,int *isNextFree)
{
    size_t  prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t  next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    *isNextFree = 0;
    /*coalesce the block and change the point*/
    if(prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        if(size>=newSIZE)
        {
            delete_node(NEXT_BLKP(bp));
            PUT(HDRP(bp), PACK(size,1));
            PUT(FTRP(bp), PACK(size,1));
            *isNextFree = 1;
        }
    }
    else if(!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        if(size>=newSIZE)
        {
            delete_node(PREV_BLKP(bp));
            PUT(FTRP(bp),PACK(size,1));
            PUT(HDRP(PREV_BLKP(bp)),PACK(size,1));
            bp = PREV_BLKP(bp);
        }
    }
    else if(!prev_alloc && !next_alloc)
    {
        size +=GET_SIZE(FTRP(NEXT_BLKP(bp)))+ GET_SIZE(HDRP(PREV_BLKP(bp)));
        if(size>=newSIZE)
        {
            delete_node(PREV_BLKP(bp));
            delete_node(NEXT_BLKP(bp));
            PUT(FTRP(NEXT_BLKP(bp)),PACK(size,1));
            PUT(HDRP(PREV_BLKP(bp)),PACK(size,1));
            bp = PREV_BLKP(bp);
        }
    }
    return bp;
}

/* Just change the size of ptr */
static void realloc_place(void *bp,size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp),PACK(csize,1));
    PUT(FTRP(bp),PACK(csize,1));
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
       return mm_malloc(size);
    if (size == 0) {
       mm_free(ptr);
       return NULL;
    }

    void *newptr;
    size_t asize, oldsize;
    oldsize = GET_SIZE(HDRP(ptr));
    asize = get_asize(size);

    if (oldsize>asize) {
        realloc_place(ptr,asize);
        return ptr;
    }
    else if (oldsize<asize) {
        int isnextFree;
        char *bp = realloc_coalesce(ptr,asize,&isnextFree);
        if(isnextFree==1){ /* If next block is free */
            realloc_place(bp,asize);
        }
        else if (isnextFree == 0 && bp != ptr) { /* If previous block is free, move the pointer and the payload */
            memmove(bp, ptr, size);
            realloc_place(bp,asize);
        }
        else { /* If realloc_coalesce fails */
            newptr = mm_malloc(size);
            memmove(newptr, ptr, size);
            mm_free(ptr);
            return newptr;
        }
        return bp;
    }
    return ptr; 
}

