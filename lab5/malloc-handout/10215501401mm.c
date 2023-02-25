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
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define WSIZE 4   /* Word and header/footer size (bytes) */
#define DSIZE 8   /* Double word size (bytes)  */
#define CHUNKSIZE (1<<12) //一块大小为4096  /* Extend heap by this amount (bytes) */

#define MAX(x,y) ((x) > (y)? (x):(y))

/* Pack a size and allocated bit into a word */
#define PACK(size,alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p,val) (*(unsigned int *)(p)=(val))
#define GET_ADDRESS(p) (*(void **)(p))

//总size,包括头尾
/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE) //头部的指针
#define FTRP(bp) ((char *)(bp)+ GET_SIZE(HDRP(bp))-DSIZE) //脚部的指针
#define PRED(bp) ((char *)(bp))       //祖先节点
#define SUCC(bp) ((char *)(bp))       //后继节点，只留后继结点

/* 获取有效字节，即获取总的size数-头尾指针(因为busyblock没有前继和后继指针) */
#define GET_PAYLOAD(bp) (GET_SIZE(HDRP(bp))-DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp)+GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))
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
 * mm_init - initialize the malloc package.
 */

//扩展堆的大小
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size=(words %2)? (words+1)*WSIZE: words*WSIZE;
    if((long)(bp=mem_sbrk(size))==-1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp),PACK(size,0));  /* Free block header */
    PUT(FTRP(bp),PACK(size,0));  /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));   /* New epilogue header */


    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

static void *coalesce(void *bp)
{
    //关于这一块的改free操作已经在free函数的过程中执行了

    size_t prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size=GET_SIZE(HDRP(bp));

    //情况一，前一块和后一块都被申请了
    if(prev_alloc && next_alloc)
    {
        return bp;
    }

    //情况二，后一块是空闲的
    else if(prev_alloc && !next_alloc)
    {
        size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp),PACK(size,0));
        //改完头部大小就变了，只能直接访问尾部，对尾部进行改大小的操作
        PUT(FTRP(bp),PACK(size,0));
    }

    //情况三，前一块是空闲的
    else if(!prev_alloc && next_alloc)
    {
        size+=GET_SIZE(FTRP(PREV_BLKP(bp)));
        PUT(FTRP(bp),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp=PREV_BLKP(bp);
    }

    //情况四，前后都是空的
    else
    {
        size+=(GET_SIZE(HDRP(NEXT_BLKP(bp)))+GET_SIZE(FTRP(PREV_BLKP(bp))));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp=PREV_BLKP(bp);
    }
    return bp;
}

//寻找合适的块
static void *find_fit(size_t asize)
{
    /* First fit search */
   void *bp;

   for (bp=heap_listp; GET_SIZE(HDRP(bp))>0; bp=NEXT_BLKP(bp)) 
   {
       if (!GET_ALLOC(HDRP(bp))&&(asize<=GET_SIZE(HDRP(bp)))) 
       {
           return bp;
       }
   }

   return NULL; // No fit
}

static void place(void* bp,size_t asize)
{

    size_t csize=GET_SIZE(HDRP(bp));
    
	//分割这个块
    if ((csize-asize)>=(2*DSIZE)) 
    {
        PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));
        bp=NEXT_BLKP(bp);
        PUT(HDRP(bp),PACK(csize-asize,0));
        PUT(FTRP(bp),PACK(csize-asize,0));
    }
    else 
    {
        PUT(HDRP(bp),PACK(csize,1));
        PUT(FTRP(bp),PACK(csize,1));
    }
}

int mm_init(void)
{
    if((heap_listp=mem_sbrk(4*WSIZE))==(void *)-1)
        return -1;
    PUT(heap_listp,0);       /* Alignment padding */
    PUT(heap_listp+(1*WSIZE),PACK(DSIZE,1));  /* Prologue header */
    PUT(heap_listp+(2*WSIZE),PACK(DSIZE,1));  /* Prologue footer */
    PUT(heap_listp+(3*WSIZE),PACK(0,1));      /* Epilogue header */
    heap_listp+=(2*WSIZE);

    if (extend_heap(CHUNKSIZE/WSIZE)==NULL)
        return -1;

    return 0;

}


/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;   /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious requests */
    if(size==0)
    {
        return NULL;
    }

    /* Adjust block size to include overhead and alignment reqs. */
    //要加上头尾两个指针
    if(size<=DSIZE)
        asize=2*DSIZE;  //强制了最小块大小是16字节，8字节用来满足对齐要求，另外8字节用来存放头部和脚部。
    else
        asize=DSIZE*((size+(DSIZE)+(DSIZE-1))/DSIZE); //对于超过8字节的请求，一般规则是加上开销字节，然后向上舍入到最接近8的整数倍

    /* Search the free list for a fit */
    if((bp=find_fit(asize))!=NULL) { //一旦分配器调整了请求的大小，它就会搜索空闲链表，寻找一个合适的空闲块
        place(bp,asize); //如果有合适的，分配器就放置这个请求块，并可选地分割出多余的部分
        return bp; //然后返回新分配块的地址
    }

    /* No fit found. Get more memory and place the block */
    //如果分配器不能够发现一个匹配的块，那么就用一个新的空闲块来扩展堆
    extendsize=MAX(asize,CHUNKSIZE); 
    if((bp=extend_heap(extendsize/WSIZE))==NULL)
        return NULL;
  	//把请求块放置在这个新的空闲块里，可选地分割这个块，然后返回一个指针，指向新分配地块 
    place(bp,asize); 
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size=GET_SIZE(HDRP(ptr));
    //头尾归为free的block
    PUT(HDRP(ptr),PACK(size,0));
    PUT(FTRP(ptr),PACK(size,0));
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
