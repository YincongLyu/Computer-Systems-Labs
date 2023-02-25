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


#define WSIZE       4     
#define DSIZE       8        
#define CHUNKSIZE  (1<<12)   
#define MINBLOCK (DSIZE + 2*WSIZE + 2*WSIZE)  

#define MAX(x, y)  ((x) > (y) ? (x) : (y))

#define PACK(size, alloc)  ((size) | (alloc))         

#define GET(p)             (*(unsigned int *)(p))        
#define PUT(p, val)        (*(unsigned int *)(p) = (val))  
#define GETADDR(p)         (*(unsigned int **)(p))   
#define PUTADDR(p, addr)   (*(unsigned int **)(p) = (unsigned int *)(addr))  


#define GET_SIZE(p)   (GET(p) & ~0x07)   
#define GET_ALLOC(p)  (GET(p) & 0x1)      
#define HDRP(bp)     ((char*)(bp) - WSIZE)                     
#define FTRP(bp)     ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)  

#define NEXT_BLKP(bp)    ((char*)(bp) + GET_SIZE((char*)(bp) - WSIZE))  
#define PREV_BLKP(bp)    ((char*)(bp) - GET_SIZE((char*)(bp) - DSIZE)) 

#define PRED_POINT(bp)   (bp)            
#define SUCC_POINT(bp)   ((char*)(bp) + WSIZE)  

static void* heap_listp;    
static void* head_free;     

static void *extend_heap(size_t size);    
static void *find_fit(size_t size);      
static void place(void *bp, size_t size);  
static void *coalesce(void *bp);           

static void insert_freelist(void *bp);
static void remove_freelist(void *bp);
static void place_freelist(void *bp);


/* 
 * mm_init - initialize the malloc package.
 */

int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1) 
        return -1;
    PUTADDR(heap_listp, NULL);                                         
    PUT(heap_listp + 1*WSIZE, PACK(8, 1));                 
    PUT(heap_listp + 2*WSIZE, PACK(8, 1));                
    PUT(heap_listp, PACK(0, 1));                               
    head_free = heap_listp;                                           
    PUTADDR(head_free, NULL);
    heap_listp += (2*WSIZE);                               

    if (extend_heap(CHUNKSIZE) == NULL)                                      
        return -1;
  
    return 0;
}


static void insert_freelist(void *bp) {   
    if (GETADDR(head_free) == NULL) {
        PUTADDR(SUCC_POINT(bp), NULL);
        PUTADDR(PRED_POINT(bp), head_free);
        PUTADDR(head_free, bp);
    } else {
        void *tmp;
        tmp = GETADDR(head_free);
        PUTADDR(SUCC_POINT(bp), tmp);
        PUTADDR(PRED_POINT(bp), head_free);
        PUTADDR(head_free, bp);
        PUTADDR(PRED_POINT(tmp), bp);
        tmp = NULL;
    }
}


static void remove_freelist(void *bp) {
    void *pre_block, *post_block;
    pre_block = GETADDR(PRED_POINT(bp));
    post_block = GETADDR(SUCC_POINT(bp));
  
    if (pre_block == head_free) {
        PUTADDR(head_free, post_block);
    } else {
        PUTADDR(SUCC_POINT(pre_block), post_block);
    }
  
    if (post_block != NULL) {
        PUTADDR(PRED_POINT(post_block), pre_block);
    }
}


static void place_freelist(void *bp) {
    void *pre_block, *post_block, *next_bp;
                                                          
    pre_block = GETADDR(PRED_POINT(bp));
    post_block = GETADDR(SUCC_POINT(bp));
    next_bp = NEXT_BLKP(bp);
                                                                           
    PUTADDR(PRED_POINT(next_bp), pre_block);
    PUTADDR(SUCC_POINT(next_bp), post_block);
                                                           
    if (pre_block == head_free) {
        PUTADDR(head_free, next_bp);
    } else {
        PUTADDR(SUCC_POINT(pre_block), next_bp);
    }
                                                                              
    if (post_block != NULL) {
        PUTADDR(PRED_POINT(post_block), next_bp);
    }
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
   size_t asize;
    size_t expend_size;
    void *bp;
    if(size<=0)
        return NULL;
    asize=ALIGN(size+8);
    if((bp=find_fit(asize))!=NULL){
        place(bp,asize);
        return bp;
    }
    expend_size=MAX(asize,CHUNKSIZE);
    bp=extend_heap(expend_size);
    if(bp!=NULL){
        place(bp,asize);
        return bp;
    }
    return NULL;
    
}

static void *extend_heap(size_t size) {
    size_t asize;   
    void *bp;

    asize = ALIGN(size);
    if ((long)(bp = mem_sbrk(asize)) == -1)
        return NULL;

    PUT(HDRP(bp), PACK(asize, 0));                                          
    PUT(FTRP(bp), PACK(asize, 0));          
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));                              
 
    return coalesce(bp);
}


static void *find_fit(size_t size) {         
    void *curbp;
    for (curbp = GETADDR(head_free); curbp != NULL; curbp = GETADDR(SUCC_POINT(curbp))) {
        if (GET_SIZE(HDRP(curbp)) >= size)
            return curbp;
    }
    return NULL;    
} 



static void place(void *bp, size_t asize) {    
    size_t total_size = GET_SIZE(HDRP(bp));
    size_t remainder_size = total_size - asize;
    if (remainder_size >= MINBLOCK) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        void *next_bp = NEXT_BLKP(bp);
        PUT(HDRP(next_bp), PACK(remainder_size, 0));
        PUT(FTRP(next_bp), PACK(remainder_size, 0));
        place_freelist(bp);
    } else {         
        PUT(HDRP(bp), PACK(total_size, 1));
        PUT(FTRP(bp), PACK(total_size, 1));
        remove_freelist(bp);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
    
}



static void *coalesce(void *bp) {
    char *pre_block, *post_block;
    int pre_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    int post_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    if ((pre_alloc!=0) &&( post_alloc!=0)) {
        insert_freelist(bp);
        return bp; 
    } else if ((pre_alloc!=0) && (post_alloc==0)) {                                                    
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        post_block = NEXT_BLKP(bp);                                                                
        remove_freelist(post_block);
        insert_freelist(bp);
    } else if ((pre_alloc==0) && (post_alloc!=0)) {                                                    
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        bp = PREV_BLKP(bp);
        remove_freelist(bp);
        insert_freelist(bp);
    } else {                                                                                              
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        pre_block = PREV_BLKP(bp);
        post_block = NEXT_BLKP(bp);
        bp = PREV_BLKP(bp);
        remove_freelist(pre_block);
        remove_freelist(post_block);
        insert_freelist(bp);
    }
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    return bp;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t old_size, new_size, extendsize;
    void *old_ptr, *new_ptr;

    if (ptr == NULL) {
        return mm_malloc(size);
    }
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    new_size = ALIGN(size + 2*WSIZE);
    old_size = GET_SIZE(HDRP(ptr));
    old_ptr = ptr;
    if (old_size >= new_size) {
        if (old_size - new_size >= MINBLOCK) {  
            place(old_ptr, new_size);
           
            return old_ptr;
        } else {  
        
            return old_ptr;
        }
    } else {  
        if ((new_ptr = find_fit(new_size)) == NULL) { 
            extendsize = MAX(new_size, CHUNKSIZE);
            if ((new_ptr = extend_heap(extendsize)) == NULL)  
                return NULL;
        }
    place(new_ptr, new_size);
    memcpy(new_ptr, old_ptr, old_size - 2*WSIZE);
    mm_free(old_ptr);
    
    return new_ptr;
    }
}

