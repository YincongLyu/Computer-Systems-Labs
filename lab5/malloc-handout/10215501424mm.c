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
    " 10215501424",
    /* First member's full name */
    "  ",
    /* First member's email address */
    "   ",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WORD_SIZE  (4)                                      
#define DWORD_SIZE (8)                                     
#define CHUNKSIZE  (1 << 12)                                // 一个内存块的大小，4096字节
#define MAX(x,y)   ((x >= y) ? (x) : (y))                  
#define MIN(x,y)   ((x <= y) ? (x) : (y))                  
#define PACK_SIZE_ALLOC(size,alloc)  (size | alloc)         // 打包分配状态和块大小，以便在存储时使用
#define GET_WORD(p)              (*((int*)(p)))           // 指定地址读取一个字
#define PUT_WORD(p,val)          ((*((int*)(p))) = val)   // 将指定值存储到指定地址。
#define GET_SIZE(blk_ptr)        (GET_WORD((void*)blk_ptr - WORD_SIZE) & ~0x7)        // 获取块的大小 
#define GET_ALLOC(blk_ptr)       (GET_WORD((void*)blk_ptr - WORD_SIZE) &  0x1)        // 获取块的分配状态
#define GET_VALID_SIZE(blk_ptr)  (GET_SIZE(blk_ptr) - 0x8)    // 获取块的有效大小
#define HEADER_PTR(blk_ptr)      ((void*)blk_ptr - WORD_SIZE)                          // 获取块头部的指针
#define FOOTER_PTR(blk_ptr)      ((void*)blk_ptr + GET_SIZE(blk_ptr) - DWORD_SIZE)     // 获取块尾部的指针 
#define PUT_HEADER_WORD(blk_ptr,size,alloc) PUT_WORD(HEADER_PTR(blk_ptr),PACK_SIZE_ALLOC(size,alloc))           // 将分配状态和块大小写入块头部
#define PUT_FOOTER_WORD(blk_ptr,size,alloc) PUT_WORD(FOOTER_PTR(blk_ptr),PACK_SIZE_ALLOC(size,alloc))           // 将分配状态和块大小写入块尾部
#define NEXT_BLOCK_PTR(blk_ptr)  ((void*)blk_ptr + GET_SIZE(blk_ptr))                       // 获取当前块的下一个块的指针
#define PREV_BLOCK_PTR(blk_ptr)  ((void*)blk_ptr - GET_SIZE((blk_ptr - WORD_SIZE)))         // 获取当前块的上一个块的指针。
#define ROUNDUP_EIGHT_BYTES(size)     (((size + (DWORD_SIZE - 1)) / 8) * 8)                 // 向上舍入


static void* heap_list = NULL;

/*
 * extend_heap - heap_mem needed to be larger extend_word_size bytes
 */
static void* extend_heap(size_t extend_word_size)
{
    void* blk_ptr = NULL;
    size_t size;
    size = (extend_word_size % 2) ? (extend_word_size + 1) * WORD_SIZE : extend_word_size * WORD_SIZE;
    if((blk_ptr = mem_sbrk(size)) == (void*)-1)	
        return NULL;
    
    PUT_HEADER_WORD(blk_ptr,size,0);
    PUT_FOOTER_WORD(blk_ptr,size,0);
    PUT_WORD(FOOTER_PTR(blk_ptr) + WORD_SIZE,PACK_SIZE_ALLOC(0,0));
    return blk_ptr; 
}

static void* find_fit_block_ptr(size_t size)
{
    void* blk_ptr = heap_list + DWORD_SIZE,*heap_hi_address = mem_heap_hi();
    while(blk_ptr < heap_hi_address)
    {
        if(!GET_ALLOC(blk_ptr) && GET_SIZE(blk_ptr) >= size)
            return blk_ptr;
        blk_ptr = NEXT_BLOCK_PTR(blk_ptr);
    }
    return NULL;
}

static void place(void* blk_ptr,size_t size)
{
    size_t prev_size = GET_SIZE(blk_ptr),now_size = prev_size - size;
    PUT_HEADER_WORD(blk_ptr,size,1);
    PUT_FOOTER_WORD(blk_ptr,size,1);
    if(!now_size)
        return;
    void* next_ptr = NEXT_BLOCK_PTR(blk_ptr); 
    PUT_HEADER_WORD(next_ptr,now_size,0);
    PUT_FOOTER_WORD(next_ptr,now_size,0);
}

static void merge_free_fragment(void* ptr)
{
    // Front_block Used     Behind_block Used    - DOING NOTING
    void* prev_ptr,*next_ptr,*now_ptr,*heap_hi_address = mem_heap_hi();
    if(ptr > heap_list && GET_ALLOC(PREV_BLOCK_PTR(ptr)) && (NEXT_BLOCK_PTR(ptr) > heap_hi_address || GET_ALLOC(NEXT_BLOCK_PTR(ptr))))
        return;
    // Merge Free Front_blocks
    now_ptr = ptr;
    prev_ptr = PREV_BLOCK_PTR(now_ptr);
    while(prev_ptr > heap_list && !GET_ALLOC(prev_ptr))
    {
        size_t sum_size = GET_SIZE(now_ptr) + GET_SIZE(prev_ptr);
        PUT_FOOTER_WORD(now_ptr,sum_size,0);
        PUT_HEADER_WORD(prev_ptr,sum_size,0);
        now_ptr = prev_ptr;
        prev_ptr = PREV_BLOCK_PTR(prev_ptr);
    }
    // Merge Free Behind_blocks
    next_ptr = NEXT_BLOCK_PTR(ptr);
    while(next_ptr < heap_hi_address && !GET_ALLOC(next_ptr))
    {
        size_t sum_size = GET_SIZE(now_ptr) + GET_SIZE(next_ptr);
        PUT_HEADER_WORD(now_ptr,sum_size,0);
        PUT_FOOTER_WORD(next_ptr,sum_size,0);
        next_ptr = NEXT_BLOCK_PTR(next_ptr);
    }
    return;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if((heap_list = mem_sbrk(4 * WORD_SIZE)) == (void*)-1)
        return -1;
    PUT_WORD(heap_list,PACK_SIZE_ALLOC(0,0));
    PUT_WORD(heap_list + 1 * WORD_SIZE,PACK_SIZE_ALLOC(8,1)); // first two blocks start (8/1)
    PUT_WORD(heap_list + 2 * WORD_SIZE,PACK_SIZE_ALLOC(8,1)); // first two blocks end   (8/1)
    PUT_WORD(heap_list + 3 * WORD_SIZE,PACK_SIZE_ALLOC(0,1)); // end one block    end   (0/1)
    heap_list += (2 * WORD_SIZE);    // heap_list = (8/1)
    
    if(extend_heap(CHUNKSIZE / WORD_SIZE) == NULL)
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    if(!size)
        return NULL;
    void* blk_ptr = NULL;
    // ROUND_UP_FOR EVERY EIGHT BYTE + HEADER_FOOTER_COST
    size = ROUNDUP_EIGHT_BYTES(size) + 8;
    // can find fited_block  
    if((blk_ptr = find_fit_block_ptr(size)) != NULL)
    {
        place(blk_ptr,size);
        return blk_ptr;
    }
    //extend
    blk_ptr = extend_heap(MAX(CHUNKSIZE,size) / WORD_SIZE);
    if(!blk_ptr)
        return NULL;
    place(blk_ptr,size);
    return blk_ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(ptr == NULL || ptr <= heap_list || ptr >= mem_heap_hi())	
        return;
    PUT_HEADER_WORD(ptr,GET_SIZE(ptr),0);
    PUT_FOOTER_WORD(ptr,GET_SIZE(ptr),0);
    merge_free_fragment(ptr);
    return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *newptr;
    if(!size)
    {
        mm_free(ptr);
        return NULL;
    } 
    newptr = mm_malloc(size);
    if(!ptr || !newptr)  
        return newptr;
    size_t copy_size = MIN(size,GET_VALID_SIZE(ptr));
    memcpy(newptr,ptr,copy_size);
    mm_free(ptr);
    return newptr;
}
