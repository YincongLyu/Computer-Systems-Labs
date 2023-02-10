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
#define WSIZE 4
#define DSIZE 8
#define INITSIZE 16
#define MINBLOCKSIZE 16
#define MAX_NUM     20   
#define CHUNKSIZE 1<<12
#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p)=(val))
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define LISTLIMIT 20
#define seg_free_lists(index) (*(char **)(seg_free_lists + (index * DSIZE)))
#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + DSIZE)
#define PRED(ptr) ((char *)(*((unsigned long *)(ptr))))
#define SUCC(ptr) ((char *)(*((unsigned long *)(SUCC_PTR(ptr)))))
#define SET_PTR(p, ptr) (*(unsigned long *)(p) = (unsigned long)(ptr))
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "mm.h"
#include "memlib.h"
static void *seg_free_lists;
static void *segregated_free_lists;
static void *extend_heap(size_t size);
static void insert_node(void *ptr, size_t size);
static void delete_node(void *ptr);
static void *coalesce(void *ptr);
static void *place(void *ptr, size_t asize);

team_t team = {
    "ateam",
    "Harry Bovik",
    "bovik@cs.cmu.edu",
    "",
    ""
};

#define ALIGNMENT 8


/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
 */
static void *coalesce(void *ptr)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr)));
    size_t succ_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HDRP(ptr));
    if (prev_alloc && succ_alloc) return ptr;
    else if (prev_alloc && !succ_alloc) {                
        delete_node(ptr);
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(ptr), PACK(size, 0));
        PUT(FTRP(ptr), PACK(size, 0));
    } else if (!prev_alloc && succ_alloc) {       
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT(FTRP(ptr), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    } else {                                  
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    }
    insert_node(ptr, size);
    return ptr;
}
static void *extend_heap(size_t size)
{
    void *ptr;
    size_t asize = ALIGN(size);
    if ((ptr = mem_sbrk(asize)) == (void *)-1) 
        return NULL;
    PUT(HDRP(ptr), PACK(asize, 0));
    PUT(FTRP(ptr), PACK(asize, 0)); 
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));
    insert_node(ptr, asize);
    return coalesce(ptr);
}
static void insert_node(void *ptr, size_t size) {
    int list_index = 0;
    void *search_ptr = ptr;
    void *insert_ptr = NULL;
    while ((list_index < LISTLIMIT - 1) && (size > 1)) {
        size >>= 1;
        list_index++;
    }
    search_ptr = seg_free_lists(list_index);
    while (search_ptr && (size > GET_SIZE(HDRP(search_ptr)))) {
        insert_ptr = search_ptr;
        search_ptr = PRED(search_ptr);
    }
    if (search_ptr) {
        if (insert_ptr) {
            SET_PTR(PRED_PTR(ptr), search_ptr);
            SET_PTR(SUCC_PTR(search_ptr), ptr);
            SET_PTR(SUCC_PTR(ptr), insert_ptr);
            SET_PTR(PRED_PTR(insert_ptr), ptr);
        } else {
            SET_PTR(PRED_PTR(ptr), search_ptr);
            SET_PTR(SUCC_PTR(search_ptr), ptr);
            SET_PTR(SUCC_PTR(ptr), NULL);
            seg_free_lists(list_index) = ptr;
        }
    } else { 
        SET_PTR(PRED_PTR(ptr), NULL);
        SET_PTR(SUCC_PTR(ptr), NULL);
        seg_free_lists(list_index) = ptr;
    }   
    return;
} 
static void delete_node(void *ptr) {
    int list_index = 0;
    size_t size = GET_SIZE(HDRP(ptr));
    while ((list_index < LISTLIMIT - 1) && (size > 1)) {
        size >>= 1;
        list_index++;
    }
    if (PRED(ptr)) {
        if (SUCC(ptr)) {
            SET_PTR(SUCC_PTR(PRED(ptr)), SUCC(ptr)); 
            SET_PTR(PRED_PTR(SUCC(ptr)), PRED(ptr));
        } else {
            SET_PTR(SUCC_PTR(PRED(ptr)), NULL);
            seg_free_lists(list_index) = PRED(ptr); 
        }
    } else {
        if (SUCC(ptr))
            SET_PTR(PRED_PTR(SUCC(ptr)), NULL);
        else 
            seg_free_lists(list_index) = NULL;
    }
    return;
} 
int mm_init(void)
{
    int list_index = 0;         
    char *heap_start;
    if ((long)(heap_start = mem_sbrk(4 * WSIZE + MAX_NUM  * DSIZE)) == -1)
       return -1;
    segregated_free_lists = heap_start;
    for(; list_index < MAX_NUM; list_index++)
        segregated_free_lists = NULL;
    heap_start += MAX_NUM * DSIZE;
    PUT(heap_start, 0);                            
    PUT(heap_start + (1 * WSIZE), PACK(DSIZE, 1)); 
    PUT(heap_start + (2 * WSIZE), PACK(DSIZE, 1)); 
    PUT(heap_start + (3 * WSIZE), PACK(0, 1));   
    if (extend_heap(INITSIZE) == NULL)
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extend_size; 
    void *ptr = NULL; 
    if (!size) return NULL;
    if (size <= 2 * DSIZE) asize = 3 * DSIZE; 
    else asize = ALIGN(size + DSIZE);
    int list_index = 0; 
    size_t tmp = asize;
    while (list_index < MAX_NUM) {
        if ((list_index == MAX_NUM - 1) || ((tmp <= 1) && (segregated_free_lists != NULL))) {
            ptr = segregated_free_lists;
            while ((ptr != NULL) && (asize > GET_SIZE(HDRP(ptr))))
                ptr = PRED(ptr);
            if (ptr != NULL)
                break;
        }
        tmp >>= 1;
        list_index++;
    }
    if (ptr == NULL) {
        extend_size = MAX(asize, CHUNKSIZE);
        if ((ptr = extend_heap(extend_size)) == NULL)
            return NULL;
    }
    ptr = place(ptr, asize);
    return ptr;
}
static void *place(void *ptr, size_t asize)
{
    size_t size = GET_SIZE(HDRP(ptr));
    delete_node(ptr);
    if (size - asize <= DSIZE * 2) {
        PUT(HDRP(ptr), PACK(size, 1)); 
        PUT(FTRP(ptr), PACK(size, 1)); 
    }
    else if (asize >= 100) {
        PUT(HDRP(ptr), PACK(size - asize, 0));
        PUT(FTRP(ptr), PACK(size - asize, 0));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        insert_node(ptr, size - asize);
        return NEXT_BLKP(ptr);
    }
    else {
        PUT(HDRP(ptr), PACK(asize, 1)); 
        PUT(FTRP(ptr), PACK(asize, 1)); 
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(size - asize, 0)); 
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size - asize, 0)); 
        insert_node(NEXT_BLKP(ptr), size - asize);
    }
    return ptr;
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    insert_node(ptr, size);
    coalesce(ptr);
    return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *new_ptr = ptr;
    int next_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    if (!size) {
        mm_free(ptr);
        return NULL;
    }

    if (!ptr) return mm_malloc(size);

    if (size <= 2 * DSIZE) size = 3 * DSIZE;
    else size = ALIGN(size + DSIZE); 
    int rsize = GET_SIZE(HDRP(ptr)) - size;
    if (rsize >= 0) 
        return ptr;
    else if (!next_size) { 
        int extend_size = MAX(-rsize, CHUNKSIZE);
        if (extend_heap(extend_size) == NULL)
            return NULL;
        rsize += extend_size;
        delete_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(size + rsize, 1));
        PUT(FTRP(ptr), PACK(size + rsize, 1));
    }
    else if (!(GET_ALLOC(HDRP(NEXT_BLKP(ptr)))) && ((rsize + next_size) >= 0)) { 
        rsize += next_size;
        delete_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(size + rsize, 1));
        PUT(FTRP(ptr), PACK(size + rsize, 1));
    }
    else { 
        new_ptr = mm_malloc(size);
        memcpy(new_ptr, ptr, GET_SIZE(HDRP(ptr)));
        mm_free(ptr);
    }
    return new_ptr;
}














