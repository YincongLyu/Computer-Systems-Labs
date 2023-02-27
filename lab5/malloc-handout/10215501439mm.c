/*
 * 分离式自由链表+最佳式匹配策略
 *
 * 通过9个不同尺寸范围大小的链表组织空闲块；
 * 根据需要的尺寸大小比较，找到比需要尺寸大于等于的那个块（第一个块）即为相符块；
 * 倘若相符块的空闲部分大到一个规定的值，那么将重新回收利用；
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
    /* Team name */
    "10215501439 汪柔柔",
    /* First member's full name */
    "10215501439 汪柔柔",
    /* First member's email address */
    "3479026857@qq.com"
};

/* 八字节对齐 */
#define ALIGNMENT 8

/* 约到对齐字节的整数倍 */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12) /* 拓展堆的大小 */
#define MINBLOCKSIZE 16

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PACK(size, alloc) ((size) | (alloc)) /* 打包尺寸和分配字节给一个字 */

/* 读和写地址p的一个单词 */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
/* 从地址p读取size和alloc字段 */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
/* 给定块ptr bp，计算其页眉和页脚的地址*/
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
/* 给定块ptr bp，计算下一个和前一个块的地址 */
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE((char*)(bp)-DSIZE))
/* 获取和设置地址p的上一个或下一个指针 */
#define GET_PREV(p) (*(unsigned int *)(p))
#define SET_PREV(p, prev) (*(unsigned int *)(p) = (prev))
#define GET_NEXT(p) (*((unsigned int *)(p)+1))
#define SET_NEXT(p, val) (*((unsigned int *)(p)+1) = (val))

static char* heap_listp = NULL;
static char* block_list_start = NULL;

void* get_freelisthead(size_t size);
static void remove_from_free_list(void *bp);
static void insert_to_free_list(void* bp);
static void* extend_heap(size_t words);
static void* coalesce(void *bp);
static void* find_fit(size_t asize);
static void place(void* bp, size_t asize);


/*
// 对于给定的尺寸，得到自由列表的头部
*/
void* get_freelisthead(size_t size)
{
    int i = 0;
    if (size <= 32) i=0;
    else if (size <= 64) i=1;
    else if (size <= 128) i=2;
    else if (size <= 256) i=3;
    else if (size <= 512) i=4;
    else if (size <= 1024) i=5;
    else if (size <= 2048) i=6;
    else if (size <= 4096) i=7;
    else  i=8;

    return block_list_start+(i*WSIZE);
    /*block_list_start是指自由列表的第一个header，每个header占用4个字节，即返回对应尺寸的头部*/
}




/*
 // 将块从自由链表中取出
 */ 
static void remove_from_free_list(void *bp)
{
    //如果指针为空或者bp所指向的块的页眉为空，则不返回
    if (bp == NULL || GET_ALLOC(HDRP(bp)))
        return;
    
    //root为指针bp页首大小所对应的分离式自由列表的header
    void *root = get_freelisthead(GET_SIZE(HDRP(bp))); 
    //prev为bp的前一个块
    void* prev = GET_PREV(bp);
    //next为bp的后一个块
    void* next = GET_NEXT(bp);
    
    //将bp的当前状态的前后两个块设置为null
    SET_PREV(bp, NULL);
    SET_NEXT(bp, NULL);
    
    /*
    对于之前状态下的前一个块，倘若为null，说明bp！是当前分离式自由列表的header，则断开与之前状态下的后一个块的链接，并将其(前状态下的后一个块)设置为前分离式自由列表的header
    */
    if (prev == NULL)
    {
        if (next != NULL) SET_PREV(next, NULL);
        PUT(root, next);
    }
    /*
    对于之前状态下的前一个块，倘若为正常值，说明bp！不是当前分离式自由列表的header，则将之前状态下的前一块和后一块链接起来（后接前，前接后）
    */
    else
    {
        if (next != NULL) SET_PREV(next, prev);
        SET_NEXT(prev, next);
    }
}





/* 
 * 将空闲的bp块插入分离式空闲列表。
 *在每个类别中，空闲列表以由小到大的空闲大小排序。
 *当找到一个合适的自由块时，只要从开始到结束找，第一个合适的就是最合适的。
*/
static void insert_to_free_list(void* bp)
{
    if (bp == NULL)
        return;
    
    //root为指针bp页首大小所对应的分离式自由列表的header
    void* root = get_freelisthead(GET_SIZE(HDRP(bp)));
    //前一个块是root
    void* prev = root;
    //下一个块是root处存放的值
    void* next = GET(root);
    
    //倘若当前列表不为空，则一直通过传递next来找到刚好bp比next小的那个next
    while (next != NULL)
    {
        if (GET_SIZE(HDRP(next)) >= GET_SIZE(HDRP(bp))) break;
        prev = next;
        next = GET_NEXT(next);
    }
    
    /*
    倘若bp！是当前列表的第一个块，则令bp为当前列表的root，置bp的前一个块为     空，置bp的后一个块为bp的后一个块，倘若next不为空，则设置bp为其前一个块
    */
    if (prev == root)
    {
        PUT(root, bp);
        SET_PREV(bp, NULL);
        SET_NEXT(bp, next);
        if (next != NULL) SET_PREV(next, bp);
    }
    /*
    倘若bp！不是当前列表的第一个块，则以bp为主体，彼此建立链接（倘若next不为     空，则设置bp为其前一个块）
    */
    else
    {
        SET_PREV(bp, prev);
        SET_NEXT(bp, next);
        SET_NEXT(prev, bp);
        if (next != NULL) SET_PREV(next, bp);
    }
}





/*
 堆的拓展
 通过4*words大小拓展堆
 */
static void* extend_heap(size_t words)
{
    char* bp;
    size_t size;

    /* 根据words的大小和奇偶，分配偶数字节，以维持对齐状态*/
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    /*
    倘若增加size字节大小的程序数据空间失败，则返回null，
    否则增加size字节大小的程序数据空间成功
    */
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /*初始化空闲块的页首，页脚为size，和后续的页首*/
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    
    /*将bp的前后块指针设置为0*/
    SET_PREV(bp, 0);
    SET_NEXT(bp, 0);
    
    /*初始化bp的下一个块的页首为1*/
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    
    /*倘若bp块是空闲的，则合并*/
    return coalesce(bp); 
}




/*
 * 合并
 * 合并相邻的空块
 */
static void* coalesce(void *bp)
{   
    /*找到bp的前后块及其分配尺寸*/
    void* prev_bp = PREV_BLKP(bp);
    void* next_bp = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev_bp));
    size_t next_alloc = GET_ALLOC(HDRP(next_bp));
    
    /*找到bp的尺寸*/
    size_t size = GET_SIZE(HDRP(bp));
    
    /*倘若bp的前后块俱已连接，则不作为*/
    if (prev_alloc && next_alloc)
    {

    }
    
    /*倘若只是bp的后一块未连接，合并后*/
    else if (prev_alloc && !next_alloc)
    {
        /*将后一块从空闲列表中移出*/
        remove_from_free_list(next_bp);
        /*bp的size增加量为后一块的size*/
        size += GET_SIZE(HDRP(next_bp));
        /*同步bp的页首、页脚size*/
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    
    /*倘若只是bp的前一块未连接，合并前*/
    else if (!prev_alloc && next_alloc)
    {
        /*将前一块从空闲列表中移出*/
        remove_from_free_list(prev_bp);
        /*bp的size增加量为前一块的size*/
        size += GET_SIZE(HDRP(prev_bp));
        /*同步bp的页脚size*/
        PUT(FTRP(bp), PACK(size, 0));
        /*将前一块的页首设置为size*/
        PUT(HDRP(prev_bp), PACK(size, 0));
        /*令前一块为bp，即前一块消失淘汰*/
        bp = prev_bp;
    }
    
    /*倘若bp的前后均未链接，合并前后*/
    else
    {
        /*将前一块从空闲列表中移出*/
        remove_from_free_list(prev_bp);
        /*将后一块从空闲列表中移出*/
        remove_from_free_list(next_bp);
        /*size的增量为前后size总和*/
        size += GET_SIZE(HDRP(prev_bp)) + GET_SIZE(FTRP(next_bp));
        /*将前一块的页首设置为size*/
        PUT(HDRP(prev_bp), PACK(size, 0));
        /*将后一块的页脚设置为size*/
        PUT(FTRP(next_bp), PACK(size, 0));
        /*令前一块为bp，即前一块消失淘汰*/
        bp = prev_bp;
    }
    /*将bp并入空闲列表*/
    insert_to_free_list(bp);
    
    return bp;
}


/*
* 找到合适的块
*/
static void* find_fit(size_t asize)
{
    /*首先设置root为当前所寻找asize的分离式空闲列表的header，
      循环终止条件为：当root等于最后一个分离式空闲列表的header的后4字节，
      循环改变量：root下移到后一个空闲列表的头部
    */
    for (void* root = get_freelisthead(asize); root != (heap_listp-WSIZE); root+=WSIZE)
    {
        /*令bp为当前列表所指的第一个块*/
        void* bp = GET(root);
        /*当bp确实存在时*/
        while (bp)
        {
            /*找到那个size刚好大于等于asize的bp即可返回*/
            if (GET_SIZE(HDRP(bp)) >= asize) return bp;
            /*倘若这一条链表没有合适的块，则令后一块为bp，寻找下一条链表*/
            bp = GET_NEXT(bp);
        }
    }
    /*遍历所有链表均没有，则找不到合适的块*/
    return NULL;
}



/*
剔骨

从空闲列表中删除块bp时，可能bp的size比asize大，那么直接使用bp比较浪费空间，可能会进行拆分，且只在剩余部分等于或大于最小块（16字节）的大小时才拆分
 */
static void place(void* bp, size_t asize)
{
    /*获取bp的size之后，从空闲列表中移除bp*/
    size_t size = GET_SIZE(HDRP(bp));
    remove_from_free_list(bp);
    
    /*倘若size比asize大16个字节，则分离bp*/
    if ((size - asize) >= MINBLOCKSIZE) 
    {   
        /*同步bp的页眉和页脚的size为asize*/
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        /*取出（新建）bp的后一个块*/
        void* new_bp = NEXT_BLKP(bp);
        /*同步new_bp的页眉和页脚的size为size-asize*/
        PUT(HDRP(new_bp), PACK(size-asize, 0));
        PUT(FTRP(new_bp), PACK(size-asize, 0));
        /*断开new_bp与前后块的链接*/
        SET_PREV(new_bp, 0);
        SET_NEXT(new_bp, 0);
        /*对于new_bp合并邻近的空块*/
        coalesce(new_bp);
    }
    
    /*倘若size！没有比asize大16个字节，则不分离bp*/
    else 
    {
        /*同步bp的页眉和页脚的size*/
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
    }
}




/* 
 * mm_init  
 *初始化malloc包
 */
int mm_init(void)
{
    /*倘若对于分离式自由链表的12个root扩展数据空间失败，则返回-1*/
    if ((heap_listp = mem_sbrk(12*WSIZE)) == (void*)-1)
        return -1;
    /*初始化12个root*/
    PUT(heap_listp, 0);           //block size <= 32   
    PUT(heap_listp+(1*WSIZE), 0); //block size <= 64
    PUT(heap_listp+(2*WSIZE), 0); //block size <= 128
    PUT(heap_listp+(3*WSIZE), 0); //block size <= 256
    PUT(heap_listp+(4*WSIZE), 0); //block size <= 512
    PUT(heap_listp+(5*WSIZE), 0); //block size <= 1024
    PUT(heap_listp+(6*WSIZE), 0); //block size <= 2048
    PUT(heap_listp+(7*WSIZE), 0); //block size <= 4096
    PUT(heap_listp+(8*WSIZE), 0); //block size > 4096
    PUT(heap_listp+(9*WSIZE), PACK(DSIZE, 1)); //开始页眉
    PUT(heap_listp+(10*WSIZE), PACK(DSIZE, 1)); //开始页脚
    PUT(heap_listp+(11*WSIZE), PACK(0, 1)); //末尾页眉
    
    /*令block_list_start为第一个root*/
    block_list_start = heap_listp;
    /*令heap_listp为第十个root*/
    heap_listp += (10 * WSIZE);
    
    /*倘若堆扩展的2的12次方/4,结果为null，则失败返回*/
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}



/* 
 * mm_malloc -
 * 在free list中找到一个空闲块，如果没有，释放堆
 *
 */
void *mm_malloc(size_t size)
{ 
    /*已适应对齐状态的asize*/
    size_t asize; 
    /*如果不合适，则扩展堆的数量*/
    size_t extendsize; 
    char* bp;
    
    /*倘若传入尺寸大小为0，则失败*/
    if (size == 0)
        return NULL;

    /*调整块大小，以包括开销和对齐要求*/
    if (size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
    
    /*倘若找到了合适的bp，进行一遍剔骨，（避免浪费空间），再返回bp*/
    if ((bp = find_fit(asize)) != NULL)
    {
        place(bp, asize);
        return bp;
    }

    // 倘若没有合适的块，获得更多的内存和地方块
    /*比较asize和2的12次方大小，将较大者赋值*/
    extendsize = MAX(asize, CHUNKSIZE);
    /*将上述较大值对bp进行拓展*/
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    /*剔骨*/
    place(bp, asize);
    return bp;
}



/*
 * mm_free -
 * 释放一个无作为的块
 */
void mm_free(void *bp)
{
    if (bp == NULL)
        return;
    /*获取bp的尺寸*/
    size_t size = GET_SIZE(HDRP(bp));
    
    /*同步bp的页眉和页脚的尺寸为size*/
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    SET_PREV(bp, 0);
    SET_NEXT(bp, 0);
    /*将bp与相邻的空闲块合并*/
    coalesce(bp);
}




/*
 * mm_realloc 
 * 简单地用mm_malloc和mm_free实现
 */
void *mm_realloc(void *ptr, size_t size)
{
    /*保存旧指针*/
    void *oldptr = ptr;
    /*建立新指针*/
    void *newptr;
    size_t copySize;
    
    /*分配大小为size的新指针*/
    newptr = mm_malloc(size);
    
    if (newptr == NULL)
      return NULL;
    /*获取旧指针的尺寸*/
    size = GET_SIZE(HDRP(oldptr));
    /*获取新指针的尺寸*/
    copySize = GET_SIZE(HDRP(newptr));
    /*倘若新尺寸比旧尺寸更大，则将旧尺寸赋值给新尺寸*/
    if (size < copySize)
      copySize = size;
    
    /*从旧指针复制copySize-WSIZE个字节到新指针*/
    memcpy(newptr, oldptr, copySize-WSIZE);
    /*释放旧指针*/
    mm_free(oldptr);
    /*返回新指针*/
    return newptr;
}