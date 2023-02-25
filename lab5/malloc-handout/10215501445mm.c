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


#define WSIZE       4      
#define DSIZE       8     
#define BLKSIZE    16      
#define OVERHEAD    8     
#define CHUNKSIZE  (1<<8)

#define MAX(x, y) ((x) > (y)? (x) : (y))  

#define ALIGNMENT 8

#define ALIGN(size) (((size_t)(size) + (ALIGNMENT - 1)) & ~0x7)

#define PACK(size, alloc)  ((size) | (alloc)) //将块大小和使用标识符组合进一个字中

#define GET(p)       (*(unsigned int *)(p))            
#define PUT(p, val)  (*(unsigned int *)(p) = (val))  

#define GET_SIZE(p)  (GET(p) & ~0x7)                   
#define GET_ALLOC(p) (GET(p) & 0x1)                  

#define HDRP(bp)       ((char *)(bp) - WSIZE)                      
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 

#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define SIZE(bp)      (GET_SIZE(HDRP(bp)))
#define PREV_SIZE(bp) (GET_SIZE((char *)(bp) - DSIZE))
#define NEXT_SIZE(bp) (GET_SIZE((char *)(bp) + SIZE(bp) - WSIZE))
#define ALLOC(bp)     (GET_ALLOC(HDRP(bp)))
#define PREV_ALLOC(bp) (GET_ALLOC((char *)(bp) - DSIZE))
#define NEXT_ALLOC(bp) (GET_ALLOC((char *)(bp) + SIZE(bp) - WSIZE))
#define PRED(bp)      ((char *)(bp) - GET(bp))
#define SUCC(bp)      ((char *)(bp) + GET((char *)(bp) + WSIZE))

#define PUT_PRED(bp, pre)  PUT(bp, (unsigned int)((char *)(bp) - (char *)(pre)))
#define PUT_SUCC(bp, suc)  PUT((char *)(bp) + WSIZE, (unsigned int)((char *)(suc) - (char *)(bp)))


static char *heap_listp = NULL;  
static size_t *list_head; 
static size_t *list_tail; 

static void *extend_heap(size_t asize);    
static void *place(void *bp, size_t asize);
static void *find_fit(size_t asize);      
static void *coalesce(void *bp);         
static inline int findlink(size_t size);
static inline void *insert(void *bp);
static inline void delete(void *bp);

#define SIZE1 (1<<4)
#define SIZE2 (1<<5)
#define SIZE3 (1<<6)
#define SIZE4 (1<<7)
#define SIZE5 (1<<8)
#define SIZE6 (1<<9)
#define SIZE7 (1<<10)           
#define SIZE8 (1<<11)
#define SIZE9 (1<<12)
#define SIZE10 (1<<13)
#define SIZE11 (1<<14)
#define SIZE12 (1<<15)
#define SIZE13 (1<<16)
#define SIZE14 (1<<17)
#define SIZE15 (1<<18)
#define SIZE16 (1<<19)
#define SIZE17 (1<<20)          
#define SIZE18 (1<<21)
#define SIZE19 (1<<22)

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) 
{
    if ((heap_listp = mem_sbrk(40 * DSIZE + 4 * WSIZE)) == (void *)-1) 
        return -1;
    
    list_head = (size_t *)heap_listp;
    list_tail = (size_t *)(heap_listp + 20 * DSIZE);
    int i;
    for (i = 0; i < 20; ++i)
    {
        list_head[i] = list_tail[i] = (size_t)NULL;//初始化链表
    }
    heap_listp += 40 * DSIZE;   

    PUT(heap_listp, 0);//序言块首                     
    PUT(heap_listp + 1 * WSIZE, PACK(DSIZE, 1)); //序言块尾
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1)); //结尾块首
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));     //堆首指针指向序言块
    heap_listp += 2 * WSIZE;
    
    if (extend_heap(CHUNKSIZE) == NULL) 
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) 
{
    size_t asize, extendsize;     
    char *bp;      
    
    if (size + OVERHEAD <= BLKSIZE)                    
        asize = BLKSIZE;                                     
    else
        asize = DSIZE * ((size + OVERHEAD + (DSIZE - 1)) / DSIZE); 
    
    if ((bp = find_fit(asize)) != NULL) //找到一个大小类list中合适的放入
    {
        bp = place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize)) == NULL) 
    {
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
    if(bp == NULL) 
        return;

    size_t size = SIZE(bp);

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(insert(bp));
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t oldsize, asize, freesize;
    char *bpnext, *next;
    void *newptr;

    if(ptr == NULL) 
        return mm_malloc(size);
    
    if(size == 0)
    {
        mm_free(ptr);
        return NULL;
    }

    oldsize = SIZE(ptr);
    asize = ALIGN(size + OVERHEAD);
    if(oldsize >= asize)
    {
        if (oldsize - asize >= BLKSIZE)
        {
            if (!NEXT_ALLOC(ptr))
            {
                oldsize += NEXT_SIZE(ptr);
                delete(NEXT_BLKP(ptr));
            }
            PUT(HDRP(ptr), PACK(asize, 1));
            PUT(FTRP(ptr), PACK(asize, 1));
            bpnext = NEXT_BLKP(ptr);
            PUT(HDRP(bpnext), PACK(oldsize - asize, 0));
            PUT(FTRP(bpnext), PACK(oldsize - asize, 0));
            insert(bpnext);
        }
        else
        {
            PUT(HDRP(ptr), PACK( oldsize, 1));
            PUT(FTRP(ptr), PACK( oldsize, 1));
        }
        return ptr;
    }
    else if (!NEXT_ALLOC(ptr) && NEXT_SIZE(ptr) + oldsize >= asize)
    {
        next = NEXT_BLKP(ptr);
        delete(next);
        if (SIZE(next) + oldsize - asize >= BLKSIZE)
        {
            freesize = SIZE(next) + oldsize - asize;
            PUT(HDRP(ptr), PACK(asize, 1));
            PUT(FTRP(ptr), PACK(asize, 1));
            bpnext = NEXT_BLKP(ptr);
            PUT(HDRP(bpnext), PACK(freesize, 0));
            PUT(FTRP(bpnext), PACK(freesize, 0));
            insert(bpnext);
        }
        else
        {
            PUT(HDRP(ptr), PACK(SIZE(next) + oldsize, 1));
            PUT(FTRP(ptr), PACK(SIZE(next) + oldsize, 1));
        }
        return ptr;
    }
    newptr = mm_malloc(size);
    memcpy(newptr, ptr, size);
    mm_free(ptr);
    return newptr;
}


static void *coalesce(void *bp) 
{
    size_t prev_alloc = PREV_ALLOC(bp);//看前一个是否空闲
    size_t next_alloc = NEXT_ALLOC(bp);//看后一个是否空闲
    size_t size = SIZE(bp);//获取块的大小

    if (prev_alloc && next_alloc)//前后都不用合并
    {
        return bp;
    }
    else if (prev_alloc && !next_alloc) //与前块合并
    { 
        size += SIZE(NEXT_BLKP(bp));
        delete(NEXT_BLKP(bp));
        delete(bp);
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert(bp);
    }
    else if (!prev_alloc && next_alloc) //与后块合并
    {     
        size += SIZE(PREV_BLKP(bp));
        delete(PREV_BLKP(bp));
        delete(bp);
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert(PREV_BLKP(bp));
        bp = PREV_BLKP(bp);
    }
    else //前后都要合并
    {                                    
        size += SIZE(PREV_BLKP(bp)) + SIZE(NEXT_BLKP(bp));
        delete(PREV_BLKP(bp));
        delete(NEXT_BLKP(bp));
        delete(bp);
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        insert(PREV_BLKP(bp));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

static void *place(void *bp, size_t asize)
{
    size_t csize = SIZE(bp);   
    void *next;
    delete(bp);
    if ((csize - asize) >= BLKSIZE) 
    { 
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        next = NEXT_BLKP(bp);
        PUT(HDRP(next), PACK(csize-asize, 0));
        PUT(FTRP(next), PACK(csize-asize, 0));
        insert(next);
    }
    else 
    {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
    return bp;
}

static void *find_fit(size_t asize)
{
    void *bp;
    int i;
    i = findlink(asize);
    for(i = findlink(asize); i<=19; ++i)
    {
        if ((void *)list_head[i] == NULL)
            continue;
        bp = (char *)list_head[i];
        while(1)
        {
            if (SIZE(bp) >= asize)
            {
                return bp;
            }
            if (bp == (void *)list_tail[i])
                break;
            bp=SUCC(bp);
        }
    }
    return NULL;
}


static inline void *insert(void *bp)
{
    int i = findlink(SIZE(bp));
    char *temp;
    if ((void *)list_head[i] == NULL)
        list_head[i] = list_tail[i] = (size_t)bp;
    else
    {
        if ((size_t)(bp) < list_head[i])//head->pre=bp;bp->next=head;head=bp
        {
            PUT_SUCC(bp, list_head[i]);
            PUT_PRED(list_head[i], bp);
            list_head[i] = (size_t)bp;
        }
        else if ((size_t)(bp) > list_tail[i])//tail->next=bp;bp->pre=tail;tail=bp
        {
            PUT_PRED(bp, list_tail[i]);
            PUT_SUCC(list_tail[i], bp);
            list_tail[i] = (size_t)bp;
        }
        else //bp->pre=temp;bp->next=temp->next;temp->next->pre=bp;temp->next=bp;
        {
            temp = (char *)list_head[i];
            while (SUCC(temp) < (char *)bp)
                temp = SUCC(temp);
            PUT_PRED(bp, temp);
            PUT_SUCC(bp, SUCC(temp));
            PUT_PRED(SUCC(temp), bp);
            PUT_SUCC(temp, bp);
        }
    }
    return bp;
}


static inline void delete(void *bp)
{
    int i;
    i = findlink(SIZE(bp));

    if (list_head[i] == list_tail[i])
    {
        list_head[i] = list_tail[i] = (size_t)NULL;
    }
    else if ((size_t)(bp) == list_head[i])//去头
        list_head[i] = (size_t)SUCC(bp);
    else if ((size_t)(bp) == list_tail[i])//去尾
        list_tail[i] = (size_t)PRED(bp);
    else //直接去掉然后相链接
    {             
        PUT_PRED(SUCC(bp), PRED(bp));
        PUT_SUCC(PRED(bp), SUCC(bp));
    }
}

static inline int findlink(size_t size)
{
    if (size <= SIZE1) 
    {
        return 0;
    } 
    else if (size <= SIZE2) 
    {
        return 1;
    } 
    else if (size <= SIZE3) 
    {
        return 2;
    } 
    else if (size <= SIZE4) 
    {
        return 3;
    } 
    else if (size <= SIZE5) 
    {
        return 4;
    } 
    else if (size <= SIZE6) 
    {
        return 5;
    } 
    else if (size <= SIZE7)
    {
        return 6;
    } 
    else if (size <= SIZE8) 
    {
        return 7;
    } 
    else if (size <= SIZE9) 
    {
        return 8;
    } 
    else if (size <= SIZE10) 
    {
        return 9;
    } 
    else if (size <= SIZE11)
    {
        return 10;
    } 
    else if (size <= SIZE12) 
    {
        return 11;
    } 
    else if (size <= SIZE13)
    {
        return 12;
    } 
    else if (size <= SIZE14)
    {
        return 13;
    } 
    else if (size <= SIZE15) 
    {
        return 14;
    } 
    else if (size <= SIZE16) 
    {
        return 15;
    }
    else if (size <= SIZE17) 
    {
        return 16;
    } 
    else if (size <= SIZE18)
    {
        return 17;
    } 
    else if (size <= SIZE19) 
    {
        return 18;
    } 
    else 
    {
        return 19;
    }
}

static void *extend_heap(size_t asize) 
{
    char *bp, *end = (char *)mem_heap_hi() - 3;
    size_t nowsize;
    if (!GET_ALLOC(end))
    {
        nowsize = asize - GET_SIZE(end);
        if ((bp = mem_sbrk(nowsize)) == (void *)-1)  
            return NULL;
        bp = end - GET_SIZE(end) + DSIZE;
        delete(bp);
    }
    else
    {
        if ((bp = mem_sbrk(asize)) == (void *)-1)  
            return NULL;
    }
    PUT(HDRP(bp), PACK(asize, 0));          
    PUT(FTRP(bp), PACK(asize, 0));        
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return insert(bp);
}
