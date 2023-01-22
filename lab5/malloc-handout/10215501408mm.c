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


/* compute the size of a word */
static const size_t word = sizeof(char *);

/*
* self-defined macros
*/
#define WSIZE word
#define DSIZE (WSIZE<<1)
#define TAGSIZE 4
#define INIT_HEAP_SIZE (1<<12)
#define LISTNUMBER 6
#define MIN_BLK_SIZE (2*TAGSIZE+2*WSIZE)

#define MAX(x, y) ((x)>(y) ? (x):(y))

#define MASK(size, is_alloc) ((size) | (is_alloc))

#define GET_MASK(p) (*(unsigned int *)(p))
#define PUT_MASK(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET_MASK(p) & ~0x7)
#define IS_ALLOC(p) (GET_MASK(p) & 0x1)

#define HDR_P(bp) ((char *)(bp) - TAGSIZE)
#define FTR_P(bp) ((char *)(bp) + GET_SIZE(HDR_P(bp)) - 2*TAGSIZE)

/* return the pointer to pre/next block in heap */
#define NXT_HEAP_BLK(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - TAGSIZE)))
#define PRE_HEAP_BLK(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - 2*TAGSIZE)))

/* return the pointer to pre/next block in free list */
#define PRE_LST_BLK(bp) (*(void **)(bp))
#define NXT_LST_BLK(bp) (*(void **)((char *)(bp) + WSIZE))

/* reset the pre/next pointer of the block pointed by bp */
#define SET_PRE_P(bp, pre) (*(void **)(bp) = (void *)(pre))
#define SET_NXT_P(bp, nxt) (*(void **)((char *)(bp) + WSIZE) = (void *)(nxt))



//macro for list operation
/* reset the list pointer at the head of the heap 
*  lp -- pointer(void **) to the pointer at the head of the heap
*/
#define SET_LIST_P(lp, bp) (*(void **)(lp) = (void *)(bp))
#define NXT_LIST_P(lp) ((void **)(lp) + 1)

/* return the n_th list pointer */
#define LIST_P(n) ((void **)mem_heap_lo() + (n))



//static inline helper function 
static inline size_t GET_BLK_SIZE(size_t size);
static inline void dislink(void *bp);
static inline int PROJ_LST(size_t size);



/*
* GET_BLK_SIZE - compute the precise size of a block given the requested size.
*/
static inline size_t GET_BLK_SIZE(size_t size) 
{
    size_t newsize = size;
    newsize = (newsize < DSIZE) ? DSIZE:newsize;
    return ALIGN(newsize+2*TAGSIZE);
}

/*
* dislink - dislink a block from its free list.
*/
static inline void dislink(void *bp) 
{
    if (!bp || IS_ALLOC(HDR_P(bp))) 
        return ;
    //exclude the prologue and epilogue
    unsigned int mask = GET_MASK(HDR_P(bp));
    if (mask == MASK(0,1) || mask == MASK(2*TAGSIZE,1))
        return ;

    char *prev_p = PRE_LST_BLK(bp);
    char *next_p = NXT_LST_BLK(bp);
    void **list_ptr = LIST_P(PROJ_LST(GET_SIZE(HDR_P(bp))));
    if (prev_p) SET_NXT_P(prev_p, next_p);
    if (next_p) SET_PRE_P(next_p, prev_p);
    if (*list_ptr == bp) 
        SET_LIST_P(list_ptr, next_p);
}

/*
* PROJ_LST - compute a list index given the size of a block.
*/
static inline int PROJ_LST(size_t size)
{
    int index = size / 64;
    int rval;
    if (index < 3) rval = index;
    else if (index == 3) rval = 2;
    else if (index >= 4 && index <= 15) rval = 3;
    else if (index >= 16 && index <= 31) rval = 4;
    else rval = 5;
    return rval;
}

/*
* mm_copy - copy size bytes from base to dest
*/
static inline void mm_copy(void *base, void *dest, size_t size)
{
    if (!(base && dest)) return ;
    size_t i;
    for (i = 0; i < size; i++) 
        *(char *)(dest+i) = *(char *)(base+i);
}




//static helper functions
static void *extend_heap(size_t size);
static void *coalesce(void *bp);
static void *find_fit(size_t size);
static void *insert_list(void *bp);
static void *place(void *bp, size_t size);
static void blk_truncate(void *bp, size_t newsize);


/*
* extend_heap - extend the heap by size bytes.
*/
static void *extend_heap(size_t size)
{
    char *mem_brk;
    if ((mem_brk = mem_sbrk(size)) == (void *)-1)
        return NULL;
    PUT_MASK(HDR_P(mem_brk), MASK(size,0));
    PUT_MASK(FTR_P(mem_brk), MASK(size,0));
    SET_PRE_P(mem_brk, NULL);
    SET_NXT_P(mem_brk, NULL);
    PUT_MASK(HDR_P(NXT_HEAP_BLK(mem_brk)), MASK(0,1));
    return coalesce(mem_brk);
}

/*
* coalesce - coalesce the block pointed by bp with potential free block around.
*            the block can either be freed or extended.
*/
static void *coalesce(void *bp)
{
    if (IS_ALLOC(HDR_P(bp))) return NULL;
    size_t pre_alloc = IS_ALLOC(HDR_P(PRE_HEAP_BLK(bp)));
    size_t nxt_alloc = IS_ALLOC(HDR_P(NXT_HEAP_BLK(bp)));

    char *nbp = bp;
    if (pre_alloc && !nxt_alloc) {
        dislink(NXT_HEAP_BLK(bp));
        size_t nsize = GET_SIZE(HDR_P(bp))+GET_SIZE(HDR_P(NXT_HEAP_BLK(bp)));
        PUT_MASK(HDR_P(bp), MASK(nsize,0));
        PUT_MASK(FTR_P(bp), MASK(nsize,0));
        nbp = insert_list(bp);
    } else if (!pre_alloc && nxt_alloc) {
        dislink(PRE_HEAP_BLK(bp));
        size_t newsize = GET_SIZE(HDR_P(bp))+GET_SIZE(HDR_P(PRE_HEAP_BLK(bp)));
        PUT_MASK(FTR_P(bp), MASK(newsize,0));
        PUT_MASK(HDR_P(PRE_HEAP_BLK(bp)), MASK(newsize,0));
        nbp = insert_list(PRE_HEAP_BLK(bp));
    } else if (!pre_alloc && !nxt_alloc) {
        dislink(NXT_HEAP_BLK(bp));
        dislink(PRE_HEAP_BLK(bp));
        size_t newsize = GET_SIZE(HDR_P(bp))+GET_SIZE(HDR_P(NXT_HEAP_BLK(bp)))+
                    GET_SIZE(HDR_P(PRE_HEAP_BLK(bp)));
        PUT_MASK(HDR_P(PRE_HEAP_BLK(bp)), MASK(newsize,0));
        PUT_MASK(FTR_P(NXT_HEAP_BLK(bp)), MASK(newsize,0));
        nbp = insert_list(PRE_HEAP_BLK(bp));
    } else {
        nbp = insert_list(bp);
    }
    return nbp;
}

/*
* find_fit - search the proper list to find a free block with enough space.
             this function does not dislink the free block.
*/
static void *find_fit(size_t size)
{
    int index = PROJ_LST(size);
    for (int i = index; i < LISTNUMBER; i++) {
        char *list_ptr = *LIST_P(i);
        //traverse the i_th list
        while (list_ptr) {
            if (GET_SIZE(HDR_P(list_ptr)) >= size) {
                dislink(list_ptr);
                return list_ptr;
            }
            list_ptr = NXT_LST_BLK(list_ptr);
        }
    }
    char *lastbp = PRE_HEAP_BLK((char *)mem_heap_hi()+1);
    size_t allocsize = size;
    if (!IS_ALLOC(HDR_P(lastbp)))
        allocsize -= GET_SIZE(HDR_P(lastbp));
    char *nbp = extend_heap(allocsize);
    dislink(nbp);
    return nbp;
}

/*
* insert_list - insert a unlinked free block into a proper list.
                Be careful, the free block must have been dislinked.
*/
static void *insert_list(void *bp)
{
    size_t size = GET_SIZE(HDR_P(bp));
    void **list_ptr = LIST_P(PROJ_LST(size));
    if (!list_ptr) return NULL;
    // check whether the list is empty 
    if (*list_ptr) SET_PRE_P(*list_ptr, bp);
    SET_PRE_P(bp, NULL);
    SET_NXT_P(bp, *list_ptr);
    SET_LIST_P(list_ptr, bp);
    return bp;
}

/*
* place - place size bytes in a linked free block and dislink the block.
          insert the rest free block might be needed.
*/
static void *place(void *bp, size_t size)
{
    if (!bp || GET_SIZE(FTR_P(bp)) < size ||
        IS_ALLOC(HDR_P(bp)))
        return NULL;
    size_t oldsize = GET_SIZE(HDR_P(bp));
    size_t rmsize = oldsize - size;
    dislink(bp);
    PUT_MASK(HDR_P(bp), MASK(oldsize,1));
    PUT_MASK(FTR_P(bp), MASK(oldsize,1));
    if (rmsize >= MIN_BLK_SIZE) {
        PUT_MASK(HDR_P(bp), MASK(size,1));
        PUT_MASK(FTR_P(bp), MASK(size,1));
        char *nbp = NXT_HEAP_BLK(bp);
        PUT_MASK(HDR_P(nbp), MASK(rmsize,0));
        PUT_MASK(FTR_P(nbp), MASK(rmsize,0));
        insert_list(nbp);
    }
    return bp;
}


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    char *mem_heap;
    size_t initsize = LISTNUMBER*WSIZE + 4*TAGSIZE;
    if ((mem_heap = mem_sbrk(initsize)) == (void *)-1)
        return -1;
    void **list_ptr = (void **)mem_heap;
    int i;
    for (i = 0; i < LISTNUMBER; i++) {
        SET_LIST_P(list_ptr, NULL);
        list_ptr = NXT_LIST_P(list_ptr);
    }
    mem_heap += LISTNUMBER*WSIZE;
    PUT_MASK(mem_heap, MASK(0,1));
    PUT_MASK(mem_heap+TAGSIZE, MASK(2*TAGSIZE,1));
    PUT_MASK(mem_heap+2*TAGSIZE, MASK(2*TAGSIZE,1));
    PUT_MASK(mem_heap+3*TAGSIZE, MASK(0,1));
    if (!extend_heap(GET_BLK_SIZE(INIT_HEAP_SIZE)))
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t newsize = GET_BLK_SIZE(size);
    void *bp = find_fit(newsize);
    return place(bp, newsize);
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if (!ptr || !IS_ALLOC(HDR_P(ptr))) 
        return ;
    size_t size = GET_SIZE(HDR_P(ptr));
    PUT_MASK(HDR_P(ptr), MASK(size,0));
    PUT_MASK(FTR_P(ptr), MASK(size,0));
    SET_PRE_P(ptr, NULL);
    SET_NXT_P(ptr, NULL);
    coalesce(ptr);
}


/*
* blk_truncate - Truncate a block with given size, 
                and add the possible free block to
                the free list.
*/
static void blk_truncate(void *bp, size_t newsize) 
{
    size_t rmsize = GET_SIZE(HDR_P(bp))-newsize;
    PUT_MASK(HDR_P(bp), MASK(newsize,1));
    PUT_MASK(FTR_P(bp), MASK(newsize,1));
    char *nbp = NXT_HEAP_BLK(bp);
    PUT_MASK(HDR_P(nbp), MASK(rmsize,0));
    PUT_MASK(FTR_P(nbp), MASK(rmsize,0));
    SET_PRE_P(nbp, NULL);
    SET_NXT_P(nbp, NULL);
    insert_list(nbp);
}



/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (!ptr) return mm_malloc(size);
    if (!size) return (mm_free(ptr), NULL);

    size_t newsize = GET_BLK_SIZE(size);
    size_t oldsize = GET_SIZE(HDR_P(ptr));
    char *rptr = NULL;
    if (oldsize < newsize) {
        char *nbp = NXT_HEAP_BLK(ptr);
        if (!IS_ALLOC(HDR_P(nbp))) {
            size_t blksize = GET_SIZE(HDR_P(ptr))+GET_SIZE(HDR_P(nbp));
            PUT_MASK(HDR_P(ptr), MASK(blksize, 1));
            PUT_MASK(FTR_P(ptr), MASK(blksize, 1));
            dislink(nbp);
        }
        if (GET_SIZE(HDR_P(ptr)) >= newsize) {
            if (GET_SIZE(HDR_P(ptr))-newsize >= MIN_BLK_SIZE) 
                blk_truncate(ptr, newsize);
            rptr = ptr;
        } else {
            rptr = find_fit(newsize);
            rptr = place(rptr, newsize);
            mm_copy(ptr, rptr, oldsize-2*TAGSIZE);
            mm_free(ptr);
        }
    } else if (oldsize >= newsize) {
        rptr = ptr;
        if (oldsize-newsize >= MIN_BLK_SIZE) 
            blk_truncate(ptr, newsize);
    }
    return rptr;
}

















