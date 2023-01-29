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
#define WSIZE       4      
#define DSIZE       8        
#define CHUNKSIZE  (1<<12)   
#define MINBLOCK (DSIZE + 2*WSIZE)

#define MAX(x, y)  ((x) > (y) ? (x) : (y))

#define PACK(size, alloc)  ((size) | (alloc))         

#define GET(p)        (*(unsigned int *)(p))          
#define PUT(p, val)   (*(unsigned int *)(p) = (val)) 

#define GET_SIZE(p)   (GET(p) & ~0x07)    
#define GET_ALLOC(p)  (GET(p) & 0x1)      
#define HDRP(bp)     ((char*)(bp) - WSIZE)                       
#define FTRP(bp)     ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 

#define NEXT_BLKP(bp)    ((char*)(bp) + GET_SIZE((char*)(bp) - WSIZE))  
#define PREV_BLKP(bp)    ((char*)(bp) - GET_SIZE((char*)(bp) - DSIZE))  

static void* heap_listp;   
static void *extend_heap(size_t size);     
static void *find_fit(size_t size);        
static void place(char *bp, size_t size);  
static void *coalesce(void *bp);          
//check
static void* next_fitp; 

int mm_init(void)
{
   
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1) 
        return -1;
    PUT(heap_listp, 0);                    
    PUT(heap_listp + 1*WSIZE, PACK(8, 1));  
    PUT(heap_listp + 2*WSIZE, PACK(8, 1));  
    PUT(heap_listp, PACK(0, 1));            
    heap_listp += (2*WSIZE);    
    next_fitp=heap_listp;

    if (extend_heap(CHUNKSIZE) == NULL)   
        return -1;

    return 0;
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

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{


    size_t asize;     
    size_t extendsize; 
    void *bp = NULL;

    if (size == 0)   
        return NULL;

    asize = ALIGN(size + 2*WSIZE);
    
    if ((bp = find_fit(asize)) != NULL) {
        place((char *)bp, asize);
        return bp;
    }
    
    //无足够空间的空闲块用来分配
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize)) == NULL) {
        return NULL;
    }    
    place(bp, asize);

    return bp;
}

//放置策略搜索   首次适配搜索
static void *find_fit(size_t size) {
    char *endp, *lastp;
    next_fitp = NEXT_BLKP(next_fitp);   
    endp = (char *)mem_heap_hi() + 1;
    lastp = next_fitp;

    do {
        if (next_fitp == endp) {
            next_fitp = heap_listp;
            continue;
        }
        if (!GET_ALLOC(HDRP(next_fitp)) && (GET_SIZE(HDRP(next_fitp)) >= size)) 
            return next_fitp;
        next_fitp = NEXT_BLKP(next_fitp);
    } while(next_fitp != lastp);

    return NULL;
}

//分割空闲块
static void place(char *bp, size_t asize) {     
    size_t total_size = GET_SIZE(HDRP(bp));
    size_t remainder_size = total_size - asize;

    if (remainder_size >= MINBLOCK) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(remainder_size, 0));
        PUT(FTRP(bp), PACK(remainder_size, 0));
    } else {          //没有已分配块或空闲块可以比最小块更小
        PUT(HDRP(bp), PACK(total_size, 1));
        PUT(FTRP(bp), PACK(total_size, 1));
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
/*
* coalesce - 合并内存块
*/
static void *coalesce(void *bp) {          
    int pre_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    int post_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (pre_alloc && post_alloc) {
        return bp;
    } else if (pre_alloc && !post_alloc) {   
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    } else if (!pre_alloc && post_alloc) {   
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        if (bp == next_fitp) {
            next_fitp = PREV_BLKP(bp);
        }
        bp = PREV_BLKP(bp);
    } else {  //前后块都合并
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        if (bp == next_fitp) {
            next_fitp = PREV_BLKP(bp);
        }
        bp = PREV_BLKP(bp);
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
        if (old_size - new_size >= MINBLOCK) {  //分割内存块
            place(old_ptr, new_size);
            return old_ptr;
        } else {   //剩余块小于最小块大小，不分割
            return old_ptr;
        }
    } else {  //释放原内存块，寻找新内存块
        if ((new_ptr = find_fit(new_size)) == NULL) {  //无合适内存块
            extendsize = MAX(new_size, CHUNKSIZE);
            if ((new_ptr = extend_heap(extendsize)) == NULL)   //拓展堆空间
                return NULL;
        }
        place(new_ptr, new_size);
        memcpy(new_ptr, old_ptr, old_size - 2*WSIZE);
        mm_free(old_ptr);
        return new_ptr;
    }
}

