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
    "wowIMcmuStudent",
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
// Size of single word
#define WSIZE 4

// Size of double word
#define DSIZE 8

// Default extend head length
#define CHUNKSIZE (1<<9)

#define MAX(x, y) ((x) > (y)? (x) : (y))

// Get the size_t value of a ptr
#define GET(p) (*(size_t *)(p))

// Set the size_t value of a ptr
#define PUT(p, val) (*(size_t *)(p) = (val))

// pack the header of a block
#define PACK(size, alloc) ((size) | (alloc))

// Read the header info of a block
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

// Given an allocated ptr, read block info
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)


#define NEXT_BLOCKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLOCKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp)- DSIZE)))

// Given a free block, read next or prev node info
#define NEXT_PTR(bp) ((char *)(bp) + WSIZE)
#define PREV_PTR(bp) ((char *)(bp))
#define GET_NEXT(bp) (*(char **)(NEXT_PTR(bp)))
#define GET_PREV(bp) (*(char **)(bp))
#define PUT_PTR(p, ptr) (*(size_t *)(p) = (size_t)(ptr))

char *heap_listp;
void *extend_heap(size_t words);
void *coalesce(void *bp);

void *find_fit_node(size_t asize);
void place(void *bp, size_t asize);

void *seg_listp[13];
void insert_block_node(void *bp);
void remove_block_node(void *bp);

int get_index(size_t asize);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{   
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *) - 1)
        return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));
    heap_listp += DSIZE;

    char *bp;
    for (int i = 0; i < 13; i++)
        seg_listp[i] = NULL;
        
    if ((bp = extend_heap(CHUNKSIZE/WSIZE)) == NULL)
        return -1;

    PUT_PTR(PREV_PTR(bp), NULL);
    PUT_PTR(NEXT_PTR(bp), NULL);

    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    char* bp;

    if (size == 0)
        return NULL;
    
    // Align block to double words
    if (size <= DSIZE){
        asize = 2 * DSIZE;
    }
    else {
        asize = (((size + DSIZE) + (DSIZE - 1))/ DSIZE) * DSIZE;
    }
    
    // If theres free block, use free block first
    if ((bp = find_fit_node(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    // If no free block, allocate for new heap space
    size_t extend_size;
    extend_size = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extend_size / WSIZE)) == NULL){
        return NULL;
    }
    place(bp, asize);
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
    size = GET_SIZE(HDRP(oldptr));
    copySize = GET_SIZE(HDRP(newptr));
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize-WSIZE);
    mm_free(oldptr);
    return newptr;
}

void *extend_heap(size_t words) {

    char *bp;
    size_t size;
    size = (words % 2)? (words + 1) * WSIZE: words * WSIZE;

    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLOCKP(bp)), PACK(0, 1));
    PUT_PTR(PREV_PTR(bp), NULL);
    PUT_PTR(NEXT_PTR(bp), NULL);

    return coalesce(bp);
}

void *coalesce(void *bp) {

    size_t prev_alloc, next_alloc;
    size_t size;

    void *next_block = NEXT_BLOCKP(bp);
    void *prev_block = PREV_BLOCKP(bp);

    prev_alloc = GET_ALLOC(FTRP(PREV_BLOCKP(bp)));
    next_alloc = GET_ALLOC(HDRP(NEXT_BLOCKP(bp)));
    size = GET_SIZE(HDRP(bp));
    
    if (prev_alloc && next_alloc) {             
        insert_block_node(bp);
        return bp;
    }

    else if (prev_alloc && (!next_alloc)) {     
        size += GET_SIZE(HDRP(NEXT_BLOCKP(bp)));
        remove_block_node(next_block);
    }

    else if ((!prev_alloc) && next_alloc) {     
        size += GET_SIZE(FTRP(PREV_BLOCKP(bp)));
        remove_block_node(prev_block);
        bp = prev_block;
    } 

    else {                                      
        size += GET_SIZE(HDRP(NEXT_BLOCKP(bp)));
        size += GET_SIZE(FTRP(PREV_BLOCKP(bp)));
        remove_block_node(next_block);
        remove_block_node(prev_block);
        bp = prev_block;
    }

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    insert_block_node(bp);
    return bp;
}

void *find_fit_node(size_t asize) {

    void *bp;
    for (int index = get_index(asize); index < 13; index++) {
        bp = seg_listp[index];
        for (; bp != NULL; bp = GET_NEXT(bp)) {
            if (GET_SIZE(HDRP(bp)) >= asize)
                return bp;
        }
    }
    return NULL;
}

void place(void *bp, size_t asize) {

    size_t block_size = GET_SIZE(HDRP(bp));
    size_t split_size = block_size - asize;

    remove_block_node(bp);
    if (split_size < 16) {
        PUT(HDRP(bp), PACK(block_size, 1));
        PUT(FTRP(bp), PACK(block_size, 1));
    }
    else {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));

        bp = NEXT_BLOCKP(bp);
        PUT(HDRP(bp), PACK(split_size, 0));
        PUT(FTRP(bp), PACK(split_size, 0));
        insert_block_node(bp);
    }
}

void remove_block_node(void *bp) {

    void *prev_block = GET_PREV(bp);
    void *next_block = GET_NEXT(bp);

    size_t size = GET_SIZE(HDRP(bp));
    int index = get_index(size);

    if (!prev_block && !next_block)     
        seg_listp[index] = NULL;
    else if (!prev_block && next_block) {
        PUT_PTR(PREV_PTR(next_block), NULL);
        seg_listp[index] = next_block;
    } else if (prev_block && !next_block) {
        PUT_PTR(NEXT_PTR(prev_block), NULL);
    } else{
        PUT_PTR(PREV_PTR(next_block), prev_block);
        PUT_PTR(NEXT_PTR(prev_block), next_block);
    }
}

void insert_block_node(void *bp) {
    
    size_t size = GET_SIZE(HDRP(bp));
    int index = get_index(size);
    void *root = seg_listp[index];
    void *prev = NULL;
    void *next = root;
    while (next) {
         if (GET_SIZE(HDRP(bp)) < GET_SIZE(HDRP(next))) break;
         prev = next;
         next = GET_NEXT(next);
    }

    if (next == root)
    {
        PUT_PTR(PREV_PTR(bp), NULL);
        PUT_PTR(NEXT_PTR(bp), next);
        if (next)
            PUT_PTR(PREV_PTR(next), bp);
        seg_listp[index] = bp;
    } else
    {
        PUT_PTR(PREV_PTR(bp), prev);
        PUT_PTR(NEXT_PTR(bp), next);
        PUT_PTR(NEXT_PTR(prev), bp);
        if (next)
            PUT_PTR(PREV_PTR(next), bp);
    }
}

int get_index(size_t asize) {
    int index = 0;
    while (asize > 1 && index < 12) {
        asize >>= 1;
        index++;
    }
    return index;
}
