//Segregated Free Lists Segregated Fit
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define WSIZE     4          
#define DSIZE     8          
#define CHUNKSIZE (1<<6)
#define MAX_NUM     20      
#define MAX(x, y) ((x) > (y) ? (x) : (y)) 
#define PACK(size, alloc) ((size) | (alloc))
// 在p处读/写一个字（4个字节 即unsigned int）
#define GET(p)            (*(unsigned int *)(p))
#define PUT(p, val)       (*(unsigned int *)(p) = (val))
// 记录空闲块的前驱和后继 指针8字节 即unsigned long
#define SET_PTR(p, ptr) (*(unsigned long *)(p) = (unsigned long)(ptr))
//从p地址处读取大小和是否分配
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
// header footer地址
#define HDRP(ptr) ((char *)(ptr) - WSIZE)
#define FTRP(ptr) ((char *)(ptr) + GET_SIZE(HDRP(ptr)) - DSIZE)
// 空闲块物理上的上一个/下一个空闲块 
#define NEXT_BLKP(ptr) ((char *)(ptr) + GET_SIZE((char *)(ptr) - WSIZE))
#define PREV_BLKP(ptr) ((char *)(ptr) - GET_SIZE((char *)(ptr) - DSIZE))
// 分离链表中空闲块的前驱/后继的地址的地址
#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + DSIZE)//指针8字节
// 分离链表中空闲块的前驱/后继的地址
#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_PTR(ptr)))
#define segregated_free_lists(index) (*(char **)(segregated_free_lists + (index * DSIZE)))
team_t team = {
    /* Team name */
    "寄",
    /* First member's full name */
    "Bao",
    /* First member's email address */
    "2535633442@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

static void *extend_heap(size_t size)
{
    void *ptr; 
    size_t size;
    // 根据传入字节数奇偶, 考虑对齐 
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    
    if ((ptr = mem_sbrk(asize)) == (void *)-1)
        return NULL;
    
    PUT(HDRP(ptr), PACK(size, 0)); //header初始化
    PUT(FTRP(ptr), PACK(size, 0)); //footer初始化
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1)); //堆结尾标注
    insert_node(ptr, size);

    return coalesce(ptr);
}

static void insert_node(void *ptr, size_t size) 
{
    int list_index;
    void *p1 = ptr;
    void *p2 = NULL;
    
    if (size < 2) list_index= 0;
    else if (size < 4) list_index = 1;
    else if (size < 8) list_index = 2;
    else if (size < 1 << 4) list_index = 3;
    else if (size < 1 << 5) list_index = 4;
    else if (size < 1 << 6) list_index = 5;
    else if (size < 1 << 7) list_index = 6;
    else if (size < 1 << 8) list_index = 7;
    else if (size < 1 << 9) list_index = 8;
    else if (size < 1 << 10) list_index = 9;
    else if (size < 1 << 11) list_index = 10;
    else if (size < 1 << 12) list_index = 11;
    else if (size < 1 << 13) list_index = 12;
    else if (size < 1 << 14) list_index = 13;
    else if (size < 1 << 15) list_index = 14;
    else if (size < 1 << 16) list_index = 15;
    else if (size < 1 << 17) list_index = 16;
    else if (size < 1 << 18) list_index = 17;
    else if (size < 1 << 19 )list_index = 18;
    else list_index = 19;
    
    p1 = segregated_free_lists(list_index);
    while ((p1 != NULL) && (size > GET_SIZE(HDRP(p1)))) {
        p2 = p1;
        p1 = PRED(p1);
    }
    
    // 插入 
    if (p1 != NULL) {
        if (p2 != NULL) {
            SET_PTR(PRED_PTR(ptr), p1);
            SET_PTR(SUCC_PTR(p1), ptr);
            SET_PTR(SUCC_PTR(ptr), p2);
            SET_PTR(PRED_PTR(p2), ptr);
        } else {
            SET_PTR(PRED_PTR(ptr), p1);
            SET_PTR(SUCC_PTR(p1), ptr);
            SET_PTR(SUCC_PTR(ptr), NULL);
           segregated_free_lists(list_index) = ptr;
        }
    } else {
        if (p2 != NULL) {
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), p2);
            SET_PTR(PRED_PTR(p2), ptr);
        } else {
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), NULL);
         segregated_free_lists(list_index) = ptr;
        }
    }
    
    return;
}


static void delete_node(void *ptr) 
{
    int list_index = 0;
    size_t size = GET_SIZE(HDRP(ptr));
    
    //找到对应大小类的链表
    if (size < 2) list_index= 0;
    else if (size < 4) list_index = 1;
    else if (size < 8) list_index = 2;
    else if (size < 1 << 4) list_index = 3;
    else if (size < 1 << 5) list_index = 4;
    else if (size < 1 << 6) list_index = 5;
    else if (size < 1 << 7) list_index = 6;
    else if (size < 1 << 8) list_index = 7;
    else if (size < 1 << 9) list_index = 8;
    else if (size < 1 << 10) list_index = 9;
    else if (size < 1 << 11) list_index = 10;
    else if (size < 1 << 12) list_index = 11;
    else if (size < 1 << 13) list_index = 12;
    else if (size < 1 << 14) list_index = 13;
    else if (size < 1 << 15) list_index = 14;
    else if (size < 1 << 16) list_index = 15;
    else if (size < 1 << 17) list_index = 16;
    else if (size < 1 << 18) list_index = 17;
    else if (size < 1 << 19 )list_index = 18;
    else list_index = 19;
    
    if (PRED(ptr) != NULL) {
        if (SUCC(ptr) != NULL) {
            SET_PTR(SUCC_PTR(PRED(ptr)), SUCC(ptr));
            SET_PTR(PRED_PTR(SUCC(ptr)), PRED(ptr));
        } else {
            SET_PTR(SUCC_PTR(PRED(ptr)), NULL);
           segregated_free_lists(list_index) = PRED(ptr);
        }
    } else {
        if (SUCC(ptr) != NULL) {
            SET_PTR(PRED_PTR(SUCC(ptr)), NULL);
        } else {
           segregated_free_lists(list_index) = NULL;
        }
    }
    
    return;
}


static void *coalesce(void *ptr)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr)));
    size_t succ_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HDRP(ptr));

    //前后皆a
    if (prev_alloc && succ_alloc) return ptr;
    else if (prev_alloc && !succ_alloc) { //前a后f                  
        delete_node(ptr);
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(ptr), PACK(size, 0));
        PUT(FTRP(ptr), PACK(size, 0));
    } else if (!prev_alloc && succ_alloc) { //前f后a            
        delete_node(ptr);
        delete_node(PREV_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT(FTRP(ptr), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    } else {  //前后皆f                                     
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

static void *place(void *ptr, size_t asize)
{
    size_t size = GET_SIZE(HDRP(ptr));
    delete_node(ptr);
    
    //剩余空间不能独立 不拆分
    if (size - asize <= DSIZE * 2) {
        PUT(HDRP(ptr), PACK(size, 1)); 
        PUT(FTRP(ptr), PACK(size, 1)); 
    }
    //剩余能够独立成块 大的空闲块放在后面
    else if (asize >= 101) {
        PUT(HDRP(ptr), PACK(size - asize, 0));
        PUT(FTRP(ptr), PACK(size - asize, 0));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        insert_node(ptr, size - asize);
        return NEXT_BLKP(ptr);
    }
    //剩余能够独立成块 小的空闲块放在前面
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
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    int list_index = 0;         
    char *heap_start; //指向堆头
    
    if ((long)(heap_start = mem_sbrk(4 * WSIZE + MAX_NUM  * DSIZE)) == -1)
       return -1;
    
    // 初始化分离空闲链表
    segregated_free_lists = heap_start;
    for(; list_index < MAX_NUM; list_index++)
        segregated_free_lists(list_index) = NULL;
    heap_start += MAX_NUM * DSIZE;

    PUT(heap_start, 0);                            //堆起始位置
    PUT(heap_start + (1 * WSIZE), PACK(DSIZE, 1)); // 序言块头
    PUT(heap_start + (2 * WSIZE), PACK(DSIZE, 1)); // 序言块尾
    PUT(heap_start + (3 * WSIZE), PACK(0, 1));     // 堆结尾标注
    
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
    size_t asize;      // adjusted size
    size_t extend_size; 
    void *ptr = NULL; 
    
    if (!size) return NULL;
    
    if (size <= 2 * DSIZE) asize = 3 * DSIZE; //多了两个指针
    else asize = ALIGN(size + DSIZE);
    
    int list_index = 0; 
    size_t tmp = asize;
    // 在分离链表中寻找
    while (list_index < MAX_NUM) {
        if ((list_index == MAX_NUM - 1) || ((tmp <= 1) && (segregated_free_lists(list_index) != NULL))) {
            ptr = segregated_free_lists(list_index);
            // Ignore blocks that are too small
            while ((ptr != NULL) && (asize > GET_SIZE(HDRP(ptr))))
                ptr = PRED(ptr);
            if (ptr != NULL)
                return ptr;
        }
        
        tmp >>= 1;
        list_index++;
    }
    
    // 如果空闲块没有被找到 堆扩展
    if (ptr == NULL) {
        extend_size = MAX(asize, CHUNKSIZE);
        if ((ptr = extend_heap(extend_size)) == NULL)
            return NULL;
    }
    ptr = place(ptr, asize);
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
    if (!size) { //size=0 重新分配0字节 即free
        mm_free(ptr);
        return NULL;
    }

    if (!ptr) return mm_malloc(size); //原地址为NULL 即malloc

    if (size <= 2 * DSIZE) size = 3 * DSIZE;
    else size = ALIGN(size + DSIZE); 
    int rsize = GET_SIZE(HDRP(ptr)) - size; //重新分配后的剩余块大小
    if (rsize >= 0) //如果newsize小于原地址 直接返回
        return ptr;
    else if (!next_size) { //到达堆末
        int extend_size = MAX(-rsize, CHUNKSIZE);
        if (extend_heap(extend_size) == NULL)
            return NULL;
        rsize += extend_size;
        delete_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(size + rsize, 1));
        PUT(FTRP(ptr), PACK(size + rsize, 1));
    }
    //下一个空闲块加上后就满足条件
    else if (!(GET_ALLOC(HDRP(NEXT_BLKP(ptr)))) && ((rsize + next_size) >= 0)) { 
        rsize += next_size;
        delete_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(size + rsize, 1));
        PUT(FTRP(ptr), PACK(size + rsize, 1));
    }
    else { //只能复制
        new_ptr = mm_malloc(size);
        memcpy(new_ptr, ptr, GET_SIZE(HDRP(ptr)));
        mm_free(ptr);
    }

    return new_ptr;
}










