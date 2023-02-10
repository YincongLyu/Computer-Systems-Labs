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
#define WSIZE       4        //字、脚部或头部的大小（字节）
#define DSIZE       8        //双字大小（字节）
#define CHUNKSIZE  (1<<12)   //扩展堆时的默认大小
#define MINBLOCK (DSIZE + 2*WSIZE)

#define MAX(x, y)  ((x) > (y) ? (x) : (y))

#define PACK(size, alloc)  ((size) | (alloc))         //将 size 和 allocated bit 合并为一个字

#define GET(p)        (*(unsigned int *)(p))          //读地址p处的一个字
#define PUT(p, val)   (*(unsigned int *)(p) = (val))  //向地址p处写一个字

#define GET_SIZE(p)   (GET(p) & ~0x07)    //得到地址p处的 size
#define GET_ALLOC(p)  (GET(p) & 0x1)      //得到地址p处的 allocated bit
//block point --> bp指向有效载荷块指针
#define HDRP(bp)     ((char*)(bp) - WSIZE)                       //获得头部的地址
#define FTRP(bp)     ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)  //获得脚部的地址, 与宏定义HDRP有耦合

#define NEXT_BLKP(bp)    ((char*)(bp) + GET_SIZE((char*)(bp) - WSIZE))  //计算后块的地址
#define PREV_BLKP(bp)    ((char*)(bp) - GET_SIZE((char*)(bp) - DSIZE))  //计算前块的地址

static void* heap_listp;    //指向序言块
/* private functions */
static void *extend_heap(size_t size);     //拓展堆块
static void *find_fit(size_t size);        //寻找空闲块
static void place(char *bp, size_t size);  //分割空闲块
static void *coalesce(void *bp);           //合并空闲块
//check
/*
static void mm_check(int verbose, const char* func);                 //heap consistency checker
static void mm_checkblock(int verbose, const char* func, void *bp);
static int mm_checkheap(int verbose, const char* func);
*/
static void mm_printblock(int verbose, const char* func);

/* 
 * mm_init - initialize the malloc package.
 */
//设立序言块、结尾块，以及序言块前的对齐块（4B），总共需要4个4B的空间
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1) 
        return -1;
    PUT(heap_listp, 0);                     //堆起绐位置的对齐块，使bp对齐8字节
    PUT(heap_listp + 1*WSIZE, PACK(8, 1));  //序言块
    PUT(heap_listp + 2*WSIZE, PACK(8, 1));  //序言块
    PUT(heap_listp, PACK(0, 1));            //结尾块
    heap_listp += (2*WSIZE);     //小技巧：使heap_listp指向下一块, 即两个序主块中间

    if (extend_heap(CHUNKSIZE) == NULL)   //拓展堆块
        return -1;
    mm_printblock(VERBOSE, __func__);
    return 0;
}

static void *extend_heap(size_t size) {
    size_t asize;   
    void *bp;

    asize = ALIGN(size);
     //printf("extend %d\n", asize);
    if ((long)(bp = mem_sbrk(asize)) == -1)
        return NULL;

    PUT(HDRP(bp), PACK(asize, 0));          //HDRP(bp)指向原结尾块
    PUT(FTRP(bp), PACK(asize, 0));          
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));   //新结尾块
    return coalesce(bp);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{


    size_t asize;     //ajusted size
    size_t extendsize;  //若无适配块则拓展堆的大小
    void *bp = NULL;

    if (size == 0)    //无效的申请
        return NULL;

    asize = ALIGN(size + 2*WSIZE);
    
    if ((bp = find_fit(asize)) != NULL) {
        place((char *)bp, asize);
        mm_printblock(VERBOSE, __func__);
        return bp;
    }
    
    //无足够空间的空闲块用来分配
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize)) == NULL) {
        return NULL;
    }    
    place(bp, asize);
    mm_printblock(VERBOSE, __func__);
    return bp;
}

//放置策略搜索   首次适配搜索
static void *find_fit(size_t size) {         
    void *curbp;
    for (curbp = heap_listp; GET_SIZE(HDRP(curbp)) > 0; curbp = NEXT_BLKP(curbp)) {
        if (!GET_ALLOC(HDRP(curbp)) && (GET_SIZE(HDRP(curbp)) >= size)) return curbp;
    }
    return NULL;    //未适配
} 

//分割空闲块
static void place(char *bp, size_t asize) {     //注意最小块的限制（16B == DSIZE + 2*WSIZE == MINBLOCK）
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
    mm_printblock(VERBOSE, __func__);
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
    } else if (pre_alloc && !post_alloc) {   //与后块合并
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    } else if (!pre_alloc && post_alloc) {   //与前块合并
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        bp = PREV_BLKP(bp);
    } else {  //前后块都合并
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        bp = PREV_BLKP(bp);
    }
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));     //FTRP()与GET_SIZE()有耦合，故此时所用的SIZE已经改变

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
            mm_printblock(VERBOSE, __func__);
            return old_ptr;
        } else {   //剩余块小于最小块大小，不分割
            mm_printblock(VERBOSE, __func__);
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
        mm_printblock(VERBOSE, __func__);
        return new_ptr;
    }
}
