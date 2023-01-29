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
//采用分离的空闲链表管理空闲块，分配方式选择分离适配 

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)
#define LISTMAX 16

#define MAX(x,y) ((x)>(y)?(x):(y))
#define PACK(size,alloc) ((size)|(alloc))
#define GET(p) (*(unsigned int *)(p))
#define PUT(p,val) (*(unsigned int *)(p)=(val)) 
#define GET_SIZE(p) (GET(p)&~0x7)
#define GET_ALLOC(p) (GET(p)&0x1)
#define HDRP(bp) ((char*)(bp)-WSIZE) //头指针
#define FTRP(bp) ((char*)(bp)+GET_SIZE(HDRP(bp))-DSIZE)//脚指针
#define NEXT_BLKP(bp) ((char*)(bp)+GET_SIZE(((char*)(bp)-WSIZE)))//下一块指针
#define PREV_BLKP(bp) ((char*)(bp)-GET_SIZE(((char*)(bp)-DSIZE)))//上一块指针
#define PRED_PTR(ptr) ((char *)(ptr))//空闲块前驱指针
#define NEXT_PTR(ptr) ((char *)(ptr + WSIZE))//空闲块后继指针
#define PRED(ptr) (*(char **)(ptr)) //空闲块前驱指针指向的地址
#define NEXT(ptr) (*(char **)(ptr+WSIZE))//空闲块后继指针指向的地址
#define CASE(i) case i: return &list##i;
#define SET_PTR(p,ptr) (* (unsigned int *) (p) = (unsigned int) (ptr)) 
#define GET_PAYLOAD(bp) (GET_SIZE(HDRP(bp))-DSIZE)// 获取有效字节，即总的size-头尾指针 

void insert(void *ptr,size_t size);
static void delete(void *ptr);
static void *coalesce(void *ptr);
static void *extend_heap(size_t size);
static void *find_fit(size_t size);
static void *place(void *ptr, size_t size);
static void *recoalesce(void *bp,size_t needsize);

static char *heap_listp;
//segregate free list 
static int list0,list1,list2,list3,list4,list5,list6,list7,list8,list9,list10,list11,list12,list13,list14,list15;
static void *segregate_list(int index)
{
    switch(index)
    {
        CASE(0) CASE(1) CASE(2) CASE(3)
        CASE(4) CASE(5) CASE(6) CASE(7)
        CASE(8) CASE(9) CASE(10) CASE(11)
        CASE(12) CASE(13) CASE(14) CASE(15)
    }
    return NULL;
}


static void *extend_heap(size_t size)
{
    void *ptr;
    size=(size%2)?(size+1)*WSIZE:size*WSIZE;
    if((ptr=mem_sbrk(size))==(void *)-1)
        return NULL;
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));
    insert(ptr,size);
    return coalesce(ptr);
}

void insert(void *ptr,size_t size)
{
    int listnum=0;
    void *prev=NULL;
    void *succ=NULL;
    int tmp=size;

    while((listnum<(LISTMAX-1)) && (tmp>1))
    {
        tmp>>=1;
        listnum++;
    }
    succ=(void *)*(unsigned int *)segregate_list(listnum);
    while((succ!=NULL) && (size>GET_SIZE(HDRP(succ))))
    {
        prev=succ;
        succ=NEXT(succ);
    }

    if(prev!=NULL)
    {
        if(succ!=NULL)
        {
            SET_PTR(PRED_PTR(ptr),prev);
            SET_PTR(NEXT_PTR(prev),ptr);
            SET_PTR(NEXT_PTR(ptr),succ);
            SET_PTR(PRED_PTR(succ),ptr);
        }
        else 
        {
            SET_PTR(PRED_PTR(ptr),prev);
            SET_PTR(NEXT_PTR(prev),ptr);
            SET_PTR(NEXT_PTR(ptr),NULL);
        }
    }
    else
    {
        if(succ!=NULL)
        {
            SET_PTR(PRED_PTR(ptr),NULL);
            SET_PTR(NEXT_PTR(ptr),succ);
            SET_PTR(PRED_PTR(succ),ptr);
            SET_PTR(segregate_list(listnum),ptr);
        }
        else 
        {
            SET_PTR(PRED_PTR(ptr),NULL);
            SET_PTR(NEXT_PTR(ptr),NULL);
            SET_PTR(segregate_list(listnum),ptr);
        }
    }
}

static void delete(void *ptr)
{
    int listnum=0;
    size_t size=GET_SIZE(HDRP(ptr));
    
    while((listnum<LISTMAX-1) && (size>1))
    {
        size>>=1;
        listnum++;
    }
    
    if(PRED(ptr)!=NULL)
    {
        if(NEXT(ptr)!=NULL)
        {
            SET_PTR(NEXT_PTR(PRED(ptr)),NEXT(ptr));
            SET_PTR(PRED_PTR(NEXT(ptr)),PRED(ptr));
        }
    
        else
        {
            SET_PTR(NEXT_PTR(PRED(ptr)), NULL);
        }
    }
    else
    {
        if(NEXT(ptr)!=NULL)
        {
            SET_PTR(PRED_PTR(NEXT(ptr)),NULL);
            SET_PTR(segregate_list(listnum),NEXT(ptr));
        }
        else
        {
            SET_PTR(segregate_list(listnum), NULL);
        }
    }
}

static void *coalesce(void *ptr)
{
    int prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(ptr)));
    int succ_alloc=GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size=GET_SIZE(HDRP(ptr));
    if(prev_alloc && succ_alloc) 
        return ptr;

    else if(prev_alloc && !succ_alloc)
    {
        delete(ptr);
        delete(NEXT_BLKP(ptr));
        size+=GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(ptr),PACK(size,0));
        PUT(FTRP(ptr),PACK(size,0));
    }
    else if(!prev_alloc && succ_alloc)
    {
        delete(ptr);
        delete(PREV_BLKP(ptr));
        size+=GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT(FTRP(ptr), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        ptr=PREV_BLKP(ptr);
    }
    else
    {
        delete(ptr);
        delete(PREV_BLKP(ptr));
        delete(NEXT_BLKP(ptr));
        size+=GET_SIZE(HDRP(PREV_BLKP(ptr)))+GET_SIZE(FTRP(NEXT_BLKP(ptr)));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
        ptr=PREV_BLKP(ptr);
    }
    insert(ptr,size);
    return ptr;
}

static void *find_fit(size_t size)
{
    size_t tmp=size;
    int listnum=0;
    void* p=NULL;
    while(listnum<LISTMAX)
    {
        if((tmp<=1) && ((void *)(*(unsigned int *)(segregate_list(listnum)))!=NULL))
        {
            p=(void *)*(unsigned int *)(segregate_list(listnum));
            while((p!=NULL) && (size>GET_SIZE(HDRP(p))))
                p=NEXT(p);
            if(p!=NULL)
                break;
        }
        tmp>>=1;
        listnum++;
    }
    return p;    
}

static void *place(void *ptr, size_t size)
{
    size_t ptr_size=GET_SIZE(HDRP(ptr));
    size_t remainder=ptr_size-size;

    delete(ptr);
    if(remainder<DSIZE*2)
    {
        PUT(HDRP(ptr),PACK(ptr_size,1));
        PUT(FTRP(ptr),PACK(ptr_size,1));
    }
    else if(size>=95)
    {
        PUT(HDRP(ptr),PACK(remainder, 0));
        PUT(FTRP(ptr),PACK(remainder, 0));
        PUT(HDRP(NEXT_BLKP(ptr)),PACK(size, 1));
        PUT(FTRP(NEXT_BLKP(ptr)),PACK(size, 1));
        insert(ptr,remainder);
        return NEXT_BLKP(ptr);
    }

    else
    {
        PUT(HDRP(ptr),PACK(size,1));
        PUT(FTRP(ptr),PACK(size,1));
        PUT(HDRP(NEXT_BLKP(ptr)),PACK(remainder,0));
        PUT(FTRP(NEXT_BLKP(ptr)),PACK(remainder,0));
        insert(NEXT_BLKP(ptr),remainder);
    }
    return ptr;
}

static void *recoalesce(void *bp,size_t needsize)
{
    size_t prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size=GET_SIZE(HDRP(bp));
   
    if(prev_alloc && next_alloc) return NULL; 

    else if(prev_alloc && !next_alloc)
    {
        size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
        if(size<needsize)  return NULL;  
        else
        {
            delete(NEXT_BLKP(bp));
            PUT(HDRP(bp),PACK(size,1));
            PUT(FTRP(bp),PACK(size,1));
            return bp;
        }
    }

    else if(!prev_alloc && next_alloc)
    {
        size+=GET_SIZE(HDRP(PREV_BLKP(bp)));
        if(size<needsize)  return NULL; 
        else
        {
            size_t sizeone=GET_PAYLOAD(bp);
            void* prev_point=PREV_BLKP(bp);
            delete(prev_point);
            PUT(FTRP(bp),PACK(size,1));
            PUT(HDRP(prev_point),PACK(size,1));
            memcpy(prev_point,bp,sizeone);
            return prev_point;
        }
    }

    else
    {
        size+=(GET_SIZE(HDRP(NEXT_BLKP(bp)))+GET_SIZE(FTRP(PREV_BLKP(bp))));
        if(size<needsize)
            return NULL;
        else
        {
            size_t sizeone=GET_PAYLOAD(bp);
            void* prev_point=PREV_BLKP(bp);
            delete(prev_point);
            delete(NEXT_BLKP(bp));
            PUT(FTRP(NEXT_BLKP(bp)),PACK(size,1));
            PUT(HDRP(PREV_BLKP(bp)),PACK(size,1));
            memcpy(prev_point,bp,sizeone);
            return prev_point;
        }
    }
}




void mm_free(void *ptr)
{
    size_t size=GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    insert(ptr, size);
    coalesce(ptr);
}


int mm_init(void)
{
    for(int listnum=0;listnum<LISTMAX;listnum++)
    {
        SET_PTR(segregate_list(listnum),NULL);
    }
    
    if((heap_listp=mem_sbrk(4*WSIZE))==(void *)-1) return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp+(1*WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp+(2*WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp+(3*WSIZE), PACK(0, 1));
    heap_listp+=(2*WSIZE);

    if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
        return -1;
    return 0;
}

void *mm_malloc(size_t size)
{
    void *p=NULL;
    if(size==0) return NULL;
    if(size<=DSIZE) 
        size=2*DSIZE;
    else 
        size=DSIZE*((size+DSIZE+DSIZE-1)/DSIZE);

    if((p=find_fit(size))==NULL)
    {
        if((p=extend_heap(MAX(size, CHUNKSIZE)))==NULL)
            return NULL;
    }
    p=place(p,size);
    return p;
}


void *mm_realloc(void *ptr, size_t size)
{
    if(ptr==NULL) return mm_malloc(size);
    if(size==0)
    { 
        mm_free(ptr);
        return ptr;
    }

    size_t asize=0;
    
    if(size<=DSIZE)
        asize=2*DSIZE;
    else
        asize=DSIZE*((size+(DSIZE)+(DSIZE-1))/DSIZE);
    
    size_t oldsize=GET_PAYLOAD(ptr);
    if(oldsize<size)
    {
        void* newptr=recoalesce(ptr,asize);
        if(newptr==NULL)
        {
            newptr=mm_malloc(asize);
            memcpy(newptr,ptr,oldsize);
            mm_free(ptr);
        }
        return newptr;
    }
    else return ptr;
    
    return NULL;
}