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
    "10215501423",
    /* First member's full name */
    "ldh",
    /* First member's email address */
    "14470839150@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* 基本常量和宏 */
#define WSIZE 4             /* 字和头/脚大小(字节) */
#define DSIZE 8             /* 双字大小(字节) */
#define CHUNKSIZE (1<<12)   /* 按此数量(字节)扩展堆 */

#define MAX(x,y) ((x) > (y) ? (x) : (y))

/* 将size和alloc的位打包成一个字 */
#define PACK(size,alloc) ((size) | (alloc))

/* 读和写地址p的字 */
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p,val)  (*(unsigned int *)(p) = (val))

/* 从地址p读取大小和分配的位 */
#define GET_SIZE(p)     (GET(p) & ~0x7)
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* 给定块ptr bp,计算其头和脚的地址 */
#define HDRP(bp)    ((char *)(bp) - WSIZE)
#define FTRP(bp)    ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* 给定块ptr bp,计算下一个和上一个块的地址 */
#define NEXT_BLKP(bp)   ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)   ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* 链表操作 */
#define NEXT_LINK_RP(bp) ((char *)(bp))
#define PREV_LINK_RP(bp) ((char *)(bp) + WSIZE)

static char * heap_listp;
static char * block_list_start;

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp,size_t asize);

static void _remove(char *bp);
static void _insert(char *bp);
static char *find_list_root(size_t size);   /** 查找该大小所在的链表 **/

#define SIZE_0 1
#define SIZE_1 2
#define SIZE_2 4
#define SIZE_3 8
#define SIZE_4 16
#define SIZE_5 32
#define SIZE_6 64
#define SIZE_7 128
#define SIZE_8 256
#define SIZE_9 512
#define SIZE_10 1024
#define SIZE_11 2048
#define SIZE_12 4096

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    /* 创建初始的空堆 */
    if((heap_listp = mem_sbrk(18*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp,0);                          /* 对齐填充 */
    PUT(heap_listp + (1*WSIZE),0);              /* 2^0=1 */
    PUT(heap_listp + (2*WSIZE),0);              /* 2^1=2 */
    PUT(heap_listp + (3*WSIZE),0);              /* 2^2=4*/
    PUT(heap_listp + (4*WSIZE),0);              /* 2^3=8*/
    PUT(heap_listp + (5*WSIZE),0);              /* 2^4=16*/
    PUT(heap_listp + (6*WSIZE),0);              /* 2^5=32*/
    PUT(heap_listp + (7*WSIZE),0);              /* 2^6=64*/
    PUT(heap_listp + (8*WSIZE),0);              /* 2^7=128*/
    PUT(heap_listp + (9*WSIZE),0);              /* 2^8=256*/
    PUT(heap_listp + (10*WSIZE),0);             /* 2^9=512*/
    PUT(heap_listp + (11*WSIZE),0);             /* 2^10=1024*/
    PUT(heap_listp + (12*WSIZE),0);             /* 2^11=2048*/
    PUT(heap_listp + (13*WSIZE),0);             /* 2^12=4096*/
    PUT(heap_listp + (14*WSIZE),0);             /* >4096*/
    PUT(heap_listp + (15*WSIZE),PACK(DSIZE,1));  /* 序言头 */
    PUT(heap_listp + (16*WSIZE),PACK(DSIZE,1));  /* 序言尾 */
    PUT(heap_listp + (17*WSIZE),PACK(0,1));      /* 结尾块 */

    block_list_start = heap_listp + (1*WSIZE);

    heap_listp += (16*WSIZE);
    
    /* 用一个CHUNKSIZE字节的空闲块扩展空堆 */
    if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;

    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;       /* 调整后的块大小 */
    size_t extendsize;  /* 如果不合适，扩展堆的数量 */
    char *bp;

    /* 忽略请求 */
    if(size == 0)
        return NULL;

    /* 调整块大小以包括开销和对齐要求 */
    if(size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    /* 在空闲块中搜索合适的 */
    if((bp = find_fit(asize)) != NULL){
        place(bp,asize);
        return bp;
    }

    /* 没有找到合适的，获得更多内存并放置内存块 */
    extendsize = MAX(asize,CHUNKSIZE);
    if((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp,asize);

    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    PUT(NEXT_LINK_RP(bp),0);
    PUT(PREV_LINK_RP(bp),0);

    coalesce(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t oldsize;
    void *newptr;

    /* 如果ptr为NULL，调用等价与mm_malloc(size) */
    if(ptr == NULL) {
        newptr = mm_malloc(size);
        return newptr;
    }

    /* 如果size等于0，则调用等价与mm_free(ptr) */
    if(size == 0) {
        mm_free(ptr);
        return 0;
    }
    
    newptr = mm_malloc(size);

    if(!newptr) {
        return 0;
    }

    /* 复制旧块的数据 */
    oldsize = GET_SIZE(HDRP(ptr));
    /* 取新旧的最小值 */
    if(size < oldsize) 
        oldsize = size;
    memcpy(newptr, ptr, oldsize);

    /* free旧块. */
    mm_free(ptr);

    return newptr;
}


static void *extend_heap(size_t dwords)
{
    char *bp;
    size_t size;

    /* 分配偶数个双字保存对齐。 */
    size = (dwords % 2) ? (dwords+1) * WSIZE: dwords * WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* 初始化空闲块头/尾和结尾 */
    PUT(HDRP(bp),PACK(size,0));         /* 空闲块头 */
    PUT(FTRP(bp),PACK(size,0));         /* 空闲块脚 */
    PUT(NEXT_LINK_RP(bp),0);
    PUT(PREV_LINK_RP(bp),0);

    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));
    
    /* 如果一个块是空闲的则合并 */
    return coalesce(bp);
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if(prev_alloc && next_alloc){               /* 1)前面的块和后面的块都是已分配的 */
        
    }
    else if(prev_alloc && !next_alloc){         /* 2)前面的块是已分配的，后面的块是空闲的 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        _remove(NEXT_BLKP(bp));
        PUT(HDRP(bp),PACK(size,0));
        PUT(FTRP(bp),PACK(size,0));
    }
    else if(!prev_alloc && next_alloc){         /* 3)前面的块是空闲的，而后面的块是已分配的 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        _remove(PREV_BLKP(bp));
        PUT(FTRP(bp),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
    }
    else{                                       /* 4)前面的和后面的块都是空闲的 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)))
            + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        _remove(PREV_BLKP(bp));
        _remove(NEXT_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
    }
    _insert(bp);
    return bp;
}

static void *find_fit(size_t asize)
{
    char *root = find_list_root(asize);

    for(root;root != block_list_start+(14*WSIZE);root+=WSIZE){
        char *bp = GET(root);

        while(bp != NULL){
            if(GET_SIZE(HDRP(bp)) >= asize) return bp;
            bp = GET(NEXT_LINK_RP(bp));
        }
    }
    
    return NULL;
}

static void place(void *bp,size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    _remove(bp);
    if((csize - asize) >= (2*DSIZE)){
        PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));

        bp = NEXT_BLKP(bp);

        PUT(HDRP(bp),PACK(csize - asize,0));
        PUT(FTRP(bp),PACK(csize - asize,0));
        PUT(NEXT_LINK_RP(bp),0);
        PUT(PREV_LINK_RP(bp),0);
        coalesce(bp);
    }else{
        PUT(HDRP(bp),PACK(csize,1));
        PUT(FTRP(bp),PACK(csize,1));

    }
}

static void _remove(char *bp){

    char *root = find_list_root(GET_SIZE(HDRP(bp)));
    char *next = GET(NEXT_LINK_RP(bp));
    char *prev = GET(PREV_LINK_RP(bp));

    /* root根节点 */
    if(prev == NULL){
        if(next != NULL) PUT(PREV_LINK_RP(next),0);
        PUT(root,next);
    }else{
        if(next != NULL) PUT(PREV_LINK_RP(next),prev);
        PUT(NEXT_LINK_RP(prev),next);
    }
    PUT(NEXT_LINK_RP(bp),0);
    PUT(PREV_LINK_RP(bp),0);
}

static void _insert(char *bp){

    char *root = find_list_root(GET_SIZE(HDRP(bp)));
    char *prev = root;
    char *next = GET(root);

    //根据块大小排序，遍历到合适的位置
    while(next != NULL){
        if(GET_SIZE(HDRP(next))>=GET_SIZE(HDRP(bp))) break;
        prev = next;
        next = GET(NEXT_LINK_RP(next));
    }

    //只有一个节点的情况
    if(prev == root){
        PUT(root,bp);
        PUT(NEXT_LINK_RP(bp),next);
        PUT(PREV_LINK_RP(bp),NULL);
        if(next!=NULL) PUT(PREV_LINK_RP(next),bp);
    }else{
        PUT(NEXT_LINK_RP(prev),bp);
        PUT(PREV_LINK_RP(bp),prev);
        PUT(NEXT_LINK_RP(bp),next);
        if(next!=NULL) PUT(PREV_LINK_RP(next),bp);
    }

}

static char *find_list_root(size_t size){
    int i = 0;
    if(size <= SIZE_0) i = 0;
    else if(size <= SIZE_1) i = 1;
    else if(size <= SIZE_2) i = 2;
    else if(size <= SIZE_3) i = 3;
    else if(size <= SIZE_4) i = 4;
    else if(size <= SIZE_5) i = 5;
    else if(size <= SIZE_6) i = 6;
    else if(size <= SIZE_7) i = 7;
    else if(size <= SIZE_8) i = 8;
    else if(size <= SIZE_9) i = 9;
    else if(size <= SIZE_10) i = 10;
    else if(size <= SIZE_11) i = 11;
    else if(size <= SIZE_12) i = 12;
    else i = 13;

    return block_list_start + (i * WSIZE);
}
