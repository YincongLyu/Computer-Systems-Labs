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
    ",,,",
    /* First member's full name */
    ",,,",
    /* First member's email address */
    ",,,,",
    /* Second member's full name (leave blank if none) */
    ",,,",
    /* Second member's email address (leave blank if none) */
    ",,,"
};

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WORD_SIZE  (4)                                      // Word        4 bytes
#define DWORD_SIZE (8)                                      // Double Word 8 bytes
#define CHUNKSIZE  (1 << 12)                                // (1 << 12) = 4096

#define MAX(x,y)   ((x >= y) ? (x) : (y))                   // max
#define MIN(x,y)   ((x <= y) ? (x) : (y))                   // min

#define PACK_SIZE_ALLOC(size,alloc)  (size | alloc)         // 3 for alloc,29 for size

#define GET_WORD(p)              (*((int*)(p)))           // unsigned int 4 bytes
#define PUT_WORD(p,val)          ((*((int*)(p))) = val)   // put val

#define GET_SIZE(blk_ptr)        (GET_WORD((void*)blk_ptr - WORD_SIZE) & ~0x7)        //(~0x7 = (111..29 bits) + (000..3bits)) 
#define GET_ALLOC(blk_ptr)       (GET_WORD((void*)blk_ptr - WORD_SIZE) &  0x1)        // 0 unused 1 used
#define GET_VALID_SIZE(blk_ptr)  (GET_SIZE(blk_ptr) - 0x8)

#define HEADER_PTR(blk_ptr)      ((void*)blk_ptr - WORD_SIZE)                          // Header 4 bytes
#define FOOTER_PTR(blk_ptr)      ((void*)blk_ptr + GET_SIZE(blk_ptr) - DWORD_SIZE)     // BLK_PTR + malloced bytes = Footer_Ptr 

#define PUT_HEADER_WORD(blk_ptr,size,alloc) PUT_WORD(HEADER_PTR(blk_ptr),PACK_SIZE_ALLOC(size,alloc))           // FOR CONVENIENCE
#define PUT_FOOTER_WORD(blk_ptr,size,alloc) PUT_WORD(FOOTER_PTR(blk_ptr),PACK_SIZE_ALLOC(size,alloc))           // FOR CONVENIENCE
#define PUT_PREV_PTR_WORD(blk_ptr,address) PUT_WORD(blk_ptr,address)
#define PUT_NEXT_PTR_WORD(blk_ptr,address) PUT_WORD(blk_ptr + WORD_SIZE,address)

#define NEXT_BLOCK_PTR(blk_ptr)  ((void*)blk_ptr + GET_SIZE(blk_ptr))                       // NEXT_BLOCK_PTR = BLK_PTR + SIZE
#define PREV_BLOCK_PTR(blk_ptr)  ((void*)blk_ptr - GET_SIZE((blk_ptr - WORD_SIZE)))         // PREV_BLOCK_PTR = BLK_PTR - PREV_SIZE

#define ROUNDUP_EIGHT_BYTES(size)     (((size + (DWORD_SIZE - 1)) / 8) * 8)                 // ROUND FOR EVERY 8 BYTES

static void* heap_list = NULL;

struct block_node
{
    struct block_node* prev_ptr;
    struct block_node* next_ptr;
} block_node;

static struct blocks_list_node
{
    struct block_node start_node;
    struct block_node end_node;
} blocks_list[32];

static size_t size2index(size_t size)
{
    size_t tmpsize = 1,index = 0;
    while((tmpsize <<= 1) <= size)
        ++index;
        
    return index;
}

static void init_block_node(struct block_node* block_node)
{
    block_node->next_ptr = NULL;
    block_node->prev_ptr = NULL;
}


static void init_block_list(struct blocks_list_node* block_list)
{
    init_block_node(&block_list->start_node);
    init_block_node(&block_list->end_node);
    block_list->start_node.next_ptr = (&block_list->end_node);
    block_list->end_node.prev_ptr   = (&block_list->start_node); 
}

static void insert_put_block_ptr(struct blocks_list_node* block_list,void* block_ptr)
{
    PUT_WORD(block_ptr,(int)&block_list->start_node);
    PUT_WORD(block_ptr + WORD_SIZE,(int)block_list->start_node.next_ptr);
    
    struct block_node* block_node_ptr = (struct block_node*)block_ptr;
    block_node_ptr->next_ptr = block_list->start_node.next_ptr; 
    block_node_ptr->prev_ptr = &block_list->start_node;
    block_node_ptr->prev_ptr->next_ptr = block_node_ptr;
    block_node_ptr->next_ptr->prev_ptr = block_node_ptr;
    //printf("insert blk_node: %x,size: %d\n",(int)block_node_ptr,GET_SIZE(block_ptr));
}

static void remove_block_ptr(void* blk_node)
{
    struct block_node* block_ptr = (struct block_node*)blk_node;
    //printf("remove blk_node: %x,size: %d\n",(int)block_ptr,GET_SIZE(blk_node));
    block_ptr->prev_ptr->next_ptr = block_ptr->next_ptr;
    block_ptr->next_ptr->prev_ptr = block_ptr->prev_ptr;
}

/*
 * extend_heap - heap_mem needed to be larger extend_word_size bytes
 */
static void* extend_heap(size_t extend_word_size)
{
    //printf("extend begin\n");
    void* blk_ptr = NULL;
    size_t size;
    
    size = (extend_word_size % 2) ? (extend_word_size + 1) * WORD_SIZE : extend_word_size * WORD_SIZE;
    if((blk_ptr = mem_sbrk(size)) == (void*)-1)	
    	return NULL;
    
    size_t index = size2index(size);
    PUT_HEADER_WORD(blk_ptr,size,0);
    PUT_FOOTER_WORD(blk_ptr,size,0);
    PUT_WORD(FOOTER_PTR(blk_ptr) + WORD_SIZE,PACK_SIZE_ALLOC(0,0));
    insert_put_block_ptr(blocks_list + index,blk_ptr);
    //printf("extend end\n");
    return blk_ptr; 
}

static void* find_fit_block_ptr(size_t index,size_t size)
{
    int i;
    struct block_node* ptr,*end_ptr;
    for(i=index;i<32;++i)
    {
        ptr = blocks_list[i].start_node.next_ptr;
        end_ptr = &blocks_list[i].end_node;
        while(ptr != end_ptr)
        {
            if(GET_SIZE(((void*)ptr)) >= size)
            	return (void*)ptr;
            ptr = ptr->next_ptr;
        }
    }
    return NULL;
}

static void place(void* blk_ptr,size_t size)
{
    size_t prev_size = GET_SIZE(blk_ptr),now_size = prev_size - size;
    remove_block_ptr(blk_ptr);
    
    if(now_size <= 8)
    {
        PUT_HEADER_WORD(blk_ptr,prev_size,1);
        PUT_FOOTER_WORD(blk_ptr,prev_size,1);
        return;
    }
        
    PUT_HEADER_WORD(blk_ptr,size,1);
    PUT_FOOTER_WORD(blk_ptr,size,1);
    
    void* next_ptr = NEXT_BLOCK_PTR(blk_ptr);
    size_t index =  size2index(now_size);
    PUT_HEADER_WORD(next_ptr,now_size,0);
    PUT_FOOTER_WORD(next_ptr,now_size,0);
    insert_put_block_ptr(blocks_list + index,next_ptr);
}

static void* merge_free_fragment(void* ptr)
{
    // Front_block Used     Behind_block Used    - DOING NOTING
    void* prev_ptr,*next_ptr,*now_ptr,*heap_hi_address = mem_heap_hi();

    // Merge Free Front_blocks
    now_ptr = ptr;
    prev_ptr = PREV_BLOCK_PTR(now_ptr);
    //printf("merge free now_ptr: %x prev_ptr: %x\n",(int)now_ptr,(int)prev_ptr);
    while(prev_ptr > heap_list && !GET_ALLOC(prev_ptr))
    {
        size_t sum_size = GET_SIZE(now_ptr) + GET_SIZE(prev_ptr);
        PUT_FOOTER_WORD(now_ptr,sum_size,0);
        PUT_HEADER_WORD(prev_ptr,sum_size,0);
        remove_block_ptr(prev_ptr);
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
        remove_block_ptr(next_ptr);
        next_ptr = NEXT_BLOCK_PTR(next_ptr);
    }
    return now_ptr;
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
    
    int i;
    for(i=0;i<32;++i)
    	init_block_list(blocks_list + i);
    
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
    if(!size)	return NULL;
    //printf("malloc start\n");
    void* blk_ptr = NULL;
    
    // ROUND_UP_FOR EVERY EIGHT BYTE + HEADER_FOOTER_COST
    size = ROUNDUP_EIGHT_BYTES(size) + 8;
    
    // can find fited_block  
    size_t index = size2index(size);
    if((blk_ptr = find_fit_block_ptr(index,size)) != NULL)
    {
        place(blk_ptr,size);
        //printf("malloc find end\n");
        return blk_ptr;
    }
    
    // extend heap
    blk_ptr = extend_heap(MAX(CHUNKSIZE,size) / WORD_SIZE);
    if(!blk_ptr)	return NULL;
    
    //printf("malloc extend_heap,blk: %x\n",(int)blk_ptr);
    place(blk_ptr,size);
    //printf("malloc extend end\n");
    return blk_ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    //not allowed free
    if(ptr == NULL || ptr <= heap_list || ptr >= mem_heap_hi())	
    	return;
    	
    PUT_HEADER_WORD(ptr,GET_SIZE(ptr),0);
    PUT_FOOTER_WORD(ptr,GET_SIZE(ptr),0);
    
    //printf("free start\n");
    ptr = merge_free_fragment(ptr);
    //printf("free cant find end\n");
    if(!ptr) 	return;
    size_t size = GET_SIZE(ptr),index = size2index(size);
    insert_put_block_ptr(blocks_list + index,ptr);
    //printf("free end\n");
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
    if(!ptr || !newptr)  return newptr;
    
    size_t copy_size = MIN(size,GET_VALID_SIZE(ptr));
    memcpy(newptr,ptr,copy_size);
    mm_free(ptr);
    return newptr;
}

