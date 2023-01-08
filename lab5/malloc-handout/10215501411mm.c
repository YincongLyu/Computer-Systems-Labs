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
 * Macros and Constants defined here
 * TODO: Macros of block operations: get_p, get_size, get_s, set_p, set_s, set_size and others
**********************************************************/
// _get_size_class - Return the offset of the size subset 
#define _GET_SIZE_CLASS(sz) ({int rsb = 4; while((sz - 1) >> rsb){rsb++;};(rsb-4) < 11 ? rsb - 4 : 10;}) 
#define _GET_SIZE_CLASS_LIST_OFFSET(offset) ((void **)(_list_head + 4 * offset))
#define _GET_SIZE_CLASS_LIST(sz) ((void **)(_list_head + 4 * _GET_SIZE_CLASS(sz)))

/*
 * Block operations defined here 
 * @note parameter of macro expansion should be variable, not expression, in order to avoid side effect
 */
#define _GETPB(p_tag) (((*(size_t *)p_tag) & 4) >> 2)
#define _GETSB(p_tag) (((*(size_t *)p_tag) & 2) >> 1)
#define _GETCB(p_tag) ((*(size_t *)p_tag) & 1)

#define _SETPB(p_tag) ((*(size_t *)p_tag) = (*(size_t *)p_tag) | 4)
#define _SETSB(p_tag) ((*(size_t *)p_tag) = (*(size_t *)p_tag) | 2)
#define _SETCB(p_tag) ((*(size_t *)p_tag) = (*(size_t *)p_tag) | 1)

#define _CLEPB(p_tag) ((*(size_t *)p_tag) = (*(size_t *)p_tag) & (~4))
#define _CLESB(p_tag) ((*(size_t *)p_tag) = (*(size_t *)p_tag) & (~2))
#define _CLECB(p_tag) ((*(size_t *)p_tag) = (*(size_t *)p_tag) & (~1))

#define _GETSZ(p_tag) ((*(size_t *)p_tag) >> 3)
#define _SETSZ(p_tag, sz) (*(size_t*)p_tag) = (sz << 3) | ((*(size_t*)p_tag) & 7)

/*
 * List operations declaration
 */

/*
 * INSERT free block in FLB, with LIFO policy
 * @param (void*)p_block: the address of the free block(8 bytes aligned)
 */
#define INSERT(p_block) ({size_t sz = _GETSZ((int *)p_block);\
                        void **list = _GET_SIZE_CLASS_LIST(sz);\
                        INSERT_LIST(p_block, list);})

/*
 * INSERT free block in specified list, with LIFO policy
 * @param (void*)p_block: the address of the free block(4 bytes aligned)
 * @param (void**)p_list: the address of the specified list
*/
#define INSERT_LIST(p_block, p_list) ({*(void **)((char *)*p_list + 4) = p_block;\
                        *(void **)((char *)p_block + 4) = p_block;\
                        *(void **)((char *)p_block + 8) = *p_list;\
                        *p_list = p_block;})
/*
 * DELETE free block
 * @param (void*)p_block: the address of the free block(4 bytes aligned)
*/
static void DELETE(void *node);
/*
 * DELETE free block in specified list
 * @param (void*)p_block: the address of the free block(4 bytes aligned)
 * @param (void**)p_list: the address of the specified list
*/
static void DELETE_LIST(void *p_block, void** p_list);

/* Block operations declarations
 * Block operations:
 * block state should only be writted when pointer to the block is explicitly passed
*/

/* FIND a free block with block.size >= size, according to first fit policy
 * @param (size_t)size: the lowest bound of block size
 * @return pointer to specified block(4 bytes aligned), NULL if cannot found within all lists
 * @note FIND function do not guarantee the result block is valid
*/
static void* FIND(size_t size);

/* SPLIT free block with specified size
 * @note size(p_block) > size + 16 should be satisfied
 * @param (void*)p_block pointer to block need be splited
 * @param (size_t)size block payload size after splited
 * @return pointer to surplus block(4 bytes aligned)
*/
static void* SPLIT(void* p_block, size_t size);

/* MERGE adjacent free blocks to new block p_block3,
 * @note size(p_block3) = size(p_block1) + size(p_block2) + 8,
 * @note set block3.state according to block1 and block2
 * @param (void*)p_block1: pointer to first block
 * @param (void*)p_block2: pointer to second block
 * @return pointer to new block's head
*/
static void* MERGE(void* p_block1, void* p_block2);

/*Heap operations declaration*/

/* RESIZE heap with new space
 * @param (size_t)size size increament of heap
*/
static void RESIZE(size_t size);


#define _DEFAULT_INIT_HEAP_SIZE 4096
#define _DEFAULT_SBRK_SIZE 256



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

static void* _list_head;
static void* _heap_head;
static void* _heap_tail;


/*Unit Test Function*/
static int _test_init();
static int _test_list_op();
static int _test_split_op();
static int _test_merge_op();
static int _test_block_op();
static int _test_resize_op();

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    void *p_head = mem_sbrk(68);
    void *p_head_state = p_head + 52;
    void *p_fblock = mem_sbrk(_DEFAULT_INIT_HEAP_SIZE + 8);
    void *p_tail = mem_sbrk(8);

    _heap_head = p_head;
    _heap_tail = p_tail;

    *(unsigned*)((char*)p_head + 60) = 1;
    *(unsigned*)((char*)p_head + 64) = 1;
    *(void **)(p_head + 4) = p_head;
    *(void **)(p_head + 8) = p_head;
    void *bptr = (void *)((char *)p_head + 12);
    _list_head = bptr;
    int offset = 0;
    while (offset < 11)
    {
        void* ptr = (void *)((char *)bptr + 4 * offset);
        *(void **)ptr = p_head;
        offset++;
    }
    *(int*)p_tail = 1;
    *(int*)((char*)p_tail + 4) = 1;

    /*
     * INIT the first free block
    */
    _SETSZ(p_fblock, _DEFAULT_INIT_HEAP_SIZE);
    _SETPB(p_fblock);
    _SETSB(p_fblock);
    _CLECB(p_fblock);
    *(unsigned*)(p_fblock + _DEFAULT_INIT_HEAP_SIZE + 4) = *(unsigned*)p_fblock;
    INSERT(p_fblock);
    return 0;
}

/* 
 * mm_malloc - Find the free block in FBL, increase the heap if fail
 */
void *mm_malloc(size_t size)
{   
    void* result = NULL;
    size_t sz_align = ALIGN(size);
    int offset = _GET_SIZE_CLASS(sz_align);
    void* block = FIND(sz_align);
    if (block)
    {
        result = block + 4;
        DELETE(block);
        // Split block if block.size is enough
        if (_GETSZ(block) > sz_align + 32)
        {
            void* prev_foot = block - 4;
            void* prev_head = prev_foot - _GETSZ(prev_foot) - 4;
            void* surplus_block = SPLIT(block, sz_align);
            _SETSB(prev_foot);
            _SETSB(prev_head);
            INSERT(surplus_block);
        }
        else
        {
            void* prev_foot = block - 4;
            void* prev_head = prev_foot - _GETSZ(prev_foot) - 4;
            void* next_head = block + _GETSZ(block) + 8;
            void* next_foot = next_head + _GETSZ(next_head) + 4;
            void* block_head = block;
            void* block_foot = block_head + _GETSZ(block_head) + 4;
            _SETSB(prev_foot);
            _SETSB(prev_head);
            _SETPB(next_head);
            _SETPB(next_foot);
            _SETCB(block_head);
            _SETCB(block_foot);
        }
    }
    else
    {
        size_t re_size = size > _DEFAULT_SBRK_SIZE ? size : _DEFAULT_SBRK_SIZE;
        RESIZE(re_size);
        result = mm_malloc(sz_align);
    }
    return result;
}

/*
 * mm_free - Delete block from FBL, merge free block if need
 */
void mm_free(void *ptr)
{
    void* block_head = ptr - 4;
    void* block_foot = block_head + _GETSZ(block_head) + 4;
    void* prev_foot = block_head - 4;
    void* prev_head = prev_foot - _GETSZ(prev_foot) - 4;
    _CLECB(block_head);
    _CLECB(block_foot);
    void* current_block = block_head;
    if (_GETCB(prev_head) == 0)
    {
        DELETE(prev_head);
        current_block = MERGE(prev_head, block_head);
    }
    else
    {
        _CLESB(prev_head);
        _CLESB(prev_foot);
    }

    block_head = current_block;
    block_foot = block_head + _GETSZ(block_head) + 4;
    void* next_head = block_foot + 4;
    void* next_foot = next_head + _GETSZ(next_head) + 4;

    if (_GETCB(next_head) == 0)
    {
        DELETE(next_head);
        current_block = MERGE(block_head, next_head);
    }
    else
    {
        _CLEPB(block_head);
        _CLEPB(block_foot);
    }

    INSERT(current_block);
    return;
}

/*
 * mm_realloc - If size > oldsize, try to find adjacent free block behind ptr, if not, free old block and malloc a new block.
 *            - If size < oldsize, split block and insert surplus block in FBL
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t sz_align = ALIGN(size);
    void *oldptr = ptr;
    void *newptr = NULL;
    size_t copySize;
    void* old_head = ptr - 4;
    void* old_foot = old_head + _GETSZ(old_head) + 4;
    void* next_head = old_foot + 4;
    size_t oldsz = _GETSZ(old_head);
    if (oldsz > sz_align)
    {
        if (oldsz > sz_align + 64)
        {
            void* surplus_head = SPLIT(old_head, sz_align);
            void* next_foot = next_head + _GETSZ(next_head) + 4;
            _CLEPB(next_head);
            _CLEPB(next_foot);
            INSERT(surplus_head);
            newptr = oldptr;
        }
        else
        {
        newptr = oldptr;
        }
    }
    else
    {
        if (_GETCB(next_head) == 0 && oldsz + _GETSZ(next_head) >= sz_align)
        {
            if (oldsz + _GETSZ(next_head) > sz_align + sz_align)
            {
                DELETE(next_head);
                void* surplus_head = SPLIT(next_head, sz_align - oldsz);
                void* merge_head = MERGE(old_head, next_head);
                void* merge_foot = merge_head + _GETSZ(merge_head) + 4;
                _SETCB(merge_head);
                _SETCB(merge_foot);
                INSERT(surplus_head);
                newptr = merge_head + 4;
            }
            else
            {
                DELETE(next_head);
                void* merge_head = MERGE(old_head, next_head);
                void* merge_foot = merge_head + _GETSZ(merge_head) + 4;
                next_head = merge_foot + 4;
                void* next_foot = next_head + _GETSZ(next_head) + 4;
                _SETCB(merge_head);
                _SETCB(merge_foot);
                _SETPB(next_head);
                _SETPB(next_foot);
                newptr = merge_head + 4;
            }
        }
        else{
            newptr = mm_malloc(size);
            if (newptr == NULL)
            return NULL;
            copySize = oldsz;
            if (size < copySize)
            copySize = size;
            memcpy(newptr, oldptr, copySize);
            mm_free(oldptr);
            }
    }
    return newptr;
}


static void DELETE(void *node)
{
    size_t sz = _GETSZ(node);
    void** list_head = _GET_SIZE_CLASS_LIST(sz);
    DELETE_LIST(node, list_head);
}
static void* FIND(size_t size)
{
    size_t sz_aligned = ALIGN(size);
    unsigned offset = _GET_SIZE_CLASS(sz_aligned);
    void* block = NULL;
    while (offset < 11)
    {
        void** list = _GET_SIZE_CLASS_LIST_OFFSET(offset);
        void* current = *list;
        while (_GETSZ(current))
        {
            if (_GETSZ(current) >= sz_aligned)
            {
                block = current;
                break;
            }
            current = *(void**)(current + 8);
        }
        if (block) break;
        offset++;
    }
    return block;
}

static void DELETE_LIST(void *p_block, void** p_list)
{
    if (*(void**)(p_block + 4) == p_block)
    {
        *p_list = *(void **)(p_block + 8);
        *(void **)(*(void **)(p_block + 8) + 4) = *(void **)(p_block + 8);
    }
    else 
    {
        void *prev = *(void **)(p_block + 4);
        void *next = *(void **)(p_block + 8);
        *(void **)(prev + 8) = next;
        *(void **)(next + 4) = prev;
    }
}




static void* SPLIT(void* p_block, size_t size)
{
    size_t surplus_size = _GETSZ(p_block) - size - 8;
    void* surplus_block_head = NULL;
    void* block_head = p_block;
    void* surplus_block_foot = block_head + _GETSZ(block_head) + 4;
    void* block_foot = NULL;
    block_foot = block_head + 4 + size;
    surplus_block_head = block_foot + 4;
    //set p_block state
    _CLESB(block_head);
    _SETCB(block_head);
    _SETSZ(block_head, size);
    *(unsigned*)block_foot = *(unsigned*)block_head;
    //set surplus_block state
    _SETPB(surplus_block_foot);
    _CLECB(surplus_block_foot);
    _SETSZ(surplus_block_foot, surplus_size);
    *(unsigned*)surplus_block_head = *(unsigned*)surplus_block_foot;
    return surplus_block_head;
}

static void* MERGE(void *p_block1, void* p_block2)
{
    void* p_head = p_block1;
    void* p_foot = p_block2 + _GETSZ(p_block2) + 4;
    //st = state
    unsigned st_block1 = *(unsigned*)p_head;
    unsigned st_block2 = *(unsigned*)p_foot;
    //get new state
    unsigned st_block = (st_block1 & (~0x3)) + (st_block2 & (~0x5)) + (0x8 << 3);
    *(unsigned *)p_head = st_block;
    *(unsigned *)p_foot = st_block;
    return p_head;
}

static void RESIZE(size_t size)
{
    size_t heap_size = size;//Valid payload size
    heap_size = ALIGN(heap_size);
    void* new_heap = mem_sbrk(heap_size + 8);
    void* new_tail = mem_sbrk(8);
    *(unsigned*) new_tail = 1;
    *(unsigned*)(new_tail + 4) = 1;
    _SETSZ(new_heap, heap_size);
    _SETSB(new_heap);
    *(unsigned*)(new_heap + heap_size + 4) = *(unsigned*)(new_heap);
    void* new_block = MERGE(_heap_tail, new_heap);
    void* prev_foot = new_block - 4;
    void* prev_head = prev_foot - _GETSZ(prev_foot) - 4;
    // if previous block is free, merged
    if (_GETSB(prev_foot) == 0)
    {
        new_block = MERGE(prev_head, new_block);
    }
    INSERT(new_block);    
    // set previous block state
    prev_foot = new_block - 4;
    _CLESB(prev_foot);
    *(unsigned*)(prev_foot - _GETSZ(prev_foot) - 4) = *(unsigned *)prev_foot;
    _heap_tail = new_tail;
}

static int _test_init()
{
    fprintf(stderr, "Now Checking Init Operations\n");
    void* heap_head = _heap_head;
    void* heap_tail = _heap_tail;
    fprintf(stderr, "Checking heap head...\n");
    assert(*(unsigned*)(heap_head + 60) == 1);
    assert(*(unsigned*)(heap_head + 64) == 1);
    //Print all blocks in free blocks list and find init free block
    unsigned offset = 0;
    unsigned counter = 0;
    void* p_init = NULL;
    while (offset < 11)
    {
        void** list = _GET_SIZE_CLASS_LIST_OFFSET(offset);
        void* current = *list;
        fprintf(stderr, "Searching list with offset=%u\n", offset);
        while(_GETSZ(current))
        {
            fprintf(stderr, "Find free block with size=%u\n", _GETSZ(current));
            p_init = current;
            current = *(void **)(current + 8);
            counter++;
        }
        offset++;
    }
    fprintf(stderr, "Free block counts=%u\n", counter);
    assert(counter == 1);
    assert(heap_head + 68 == p_init);
    fprintf(stderr, "Check heap head done\n");
    fprintf(stderr, "Checking init free block...\n");
    assert(_GETSZ(p_init) == _DEFAULT_INIT_HEAP_SIZE);
    assert(_GETPB(p_init) == 1);
    assert(_GETSB(p_init) == 1);
    assert(_GETCB(p_init) == 0);
    unsigned head_state = *(unsigned*)p_init;
    unsigned foot_state = *(unsigned*)(p_init + _GETSZ(p_init) + 4);
    assert(head_state == foot_state);
    fprintf(stderr, "Check init free block done\n");
    fprintf(stderr, "Checking heap tail...\n");
    assert(p_init + _GETSZ(p_init) + 8 == heap_tail);
    assert(*(unsigned*)heap_tail == 1);
    assert(*(unsigned*)(heap_tail + 4) == 1);
    fprintf(stderr, "Check heap tail done\n");
    fprintf(stderr, "Check init operation done\n");
    return 0;
}

static int _test_list_op()
{
    fprintf(stderr, "Now Checking List Operations\n");
    void** list = mem_sbrk(4);
    void* block1 = mem_sbrk(16);
    void* block2 = mem_sbrk(16);
    void* block3 = mem_sbrk(16);
    void* block4 = mem_sbrk(16);
    void* block5 = mem_sbrk(16);
    void* tail = mem_sbrk(16);
    fprintf(stderr, "Checking Insert Operation\n");
    //Init Linklist, tail node
    fprintf(stderr, "Init list and tail node\n");
    *list = tail;
    *(int*) tail = 1;
    *(void**)(tail + 4) = tail;
    *(void**)(tail + 8) = NULL;   
    *(int*)(tail + 12) = 1;
    fprintf(stderr, "Insert block1\n");
    INSERT_LIST(block1, list);
    assert(*(void**)(block1 + 4) == block1);
    assert(*list == block1);
    assert(*(void**)(block1 + 8) == tail);
    fprintf(stderr, "Insert block2\n");
    INSERT_LIST(block2, list);
    assert(*(void**)(block2 + 4) == block2);
    assert(*(void**)(block1 + 4) == block2);
    assert(*(void**)(block2 + 8) == block1);
    assert(*list == block2);
    assert(*(void**)(block1 + 8) == tail);
    fprintf(stderr, "Check Inset Operation Done\n");
    fprintf(stderr, "Checking Delete Operation\n");
    fprintf(stderr, "Insert block3\n");
    INSERT_LIST(block3, list);
    fprintf(stderr, "Delete block2\n");
    DELETE_LIST(block2, list);
    assert(*(void**)(block3 + 8) == block1);
    assert(*(void**)(block1 + 4) == block3);
    fprintf(stderr, "Delete block3\n");
    DELETE_LIST(block3, list);
    assert(*list == block1);
    assert(*(void**)(block1 + 4) == block1);
    fprintf(stderr, "Delete block1\n");
    DELETE_LIST(block1, list);
    assert(*list == tail);
    fprintf(stderr, "Check Delete Operation Done\n");
    return 0;
}

static int _test_block_op()
{
    fprintf(stderr, "Now Checking Block Operations\n");
    fprintf(stderr, "Init block\n");
    void* block1 = mem_sbrk(264);
    _SETSZ(block1, 256);
    *(unsigned*)(block1 + _GETSZ(block1) + 4) = *(unsigned*)block1;
    void* block2 = mem_sbrk(72);
    _SETSZ(block2, 64);
    *(unsigned*)(block2 + _GETSZ(block2) + 4) = *(unsigned*)block2;
    fprintf(stderr, "Checking block operations...\n");
    INSERT(block1);
    INSERT(block2);
    assert(FIND(64) == block2);
    assert(FIND(256) == block1);
    assert(FIND(630) == NULL);
    DELETE(block1);
    DELETE(block2);
    assert(FIND(47) == FIND(512));
    void* init_block = FIND(87);
    DELETE(init_block);
    assert(FIND(45) == NULL);
    fprintf(stderr, "Check block operations done\n");
    return 0;
}

static int _test_split_op()
{
    fprintf(stderr, "Now Checking Split Operation\n");
    //Init Block
    fprintf(stderr, "Init blocks\n");
    void* block = mem_sbrk(264);
    _SETSZ(block, 256);
    _SETPB(block);
    _SETSB(block);
    _CLECB(block);
    *(unsigned*)(block + _GETSZ(block) + 4) = *(unsigned*)block;
    fprintf(stderr, "Spliting\n");
    void* sub_block = SPLIT(block, 128);
    fprintf(stderr, "Checking block...\n");
    assert(_GETSZ(block) == 128);
    assert(_GETPB(block) == 1);
    assert(_GETSB(block) == 0);
    assert(_GETCB(block) == 0);
    assert(*(unsigned*)block == *(unsigned*)(block + _GETSZ(block) + 4));
    fprintf(stderr, "Check block done\n");
    fprintf(stderr, "Checking surplus block...\n");
    assert(sub_block == block + _GETSZ(block) + 8);
    fprintf(stderr, "Left size:%u\n", _GETSZ(sub_block));
    assert(_GETSZ(sub_block) + _GETSZ(block) + 8 + 8 == 264);
    assert(_GETSZ(sub_block) + 128 + 8 + 8 == 264);
    assert(_GETPB(sub_block) == 1);
    assert(_GETSB(sub_block) == 1);
    assert(_GETCB(sub_block) == 0);
    assert(*(unsigned*)sub_block == *(unsigned*)(sub_block + _GETSZ(sub_block) + 4));
    fprintf(stderr, "Check surplus block done\n");
    fprintf(stderr, "Check split operation done\n");
    return 0;
}

static int _test_merge_op()
{
    fprintf(stderr, "Now Checking Merge Operation\n");
    //Init Block
    fprintf(stderr, "Init blocks with prev=1, succ=1\n");
    void* block1 = mem_sbrk(72);
    void* block2 = mem_sbrk(48);
    _SETSZ(block1, 64);
    void* block1_foot = block1 + 64 + 4;
    void* block2_foot = block2 + 40 + 4;
    _SETSZ(block1_foot, 64);
    _SETPB(block1);
    _SETPB(block1_foot);
    _SETSZ(block2, 40);
    _SETSZ(block2_foot, 40);
    _CLESB(block2);
    _CLESB(block2_foot);
    fprintf(stderr, "Merging\n");
    void* block3 = MERGE(block1, block2);
    assert(_GETSZ(block3) == 64 + 40 + 8);
    void* block3_foot =  (block3 + _GETSZ(block3) + 4);
    assert(_GETSZ(block3_foot) == 64 + 40 +8);
    assert(_GETPB(block3) == 1);
    assert(_GETPB(block3_foot)== 1);
    assert(_GETSB(block3) == 0);
    assert(_GETSB(block3_foot) == 0);
    fprintf(stderr, "Check Merge Operation Done\n");
    return 0;
}

static int _test_resize_op()
{
    fprintf(stderr, "Now Checking Resize Operation\n");
    size_t heap_size = abs(_heap_tail - _heap_head) - 68 - 8;
    RESIZE(heap_size);
    //Print all blocks in free blocks list
    unsigned offset = 0;
    unsigned counter = 0;
    while (offset < 11)
    {
        void** list = _GET_SIZE_CLASS_LIST_OFFSET(offset);
        void* current = *list;
        fprintf(stderr, "Searching list with offset=%u\n", offset);
        while(_GETSZ(current))
        {
            fprintf(stderr, "Find free block with size=%u\n", _GETSZ(current));
            current = *(void **)(current + 8);
            counter++;
        }
        offset++;
    }
    fprintf(stderr, "Free block counts=%u\n", counter);
    assert(counter == 2);
    fprintf(stderr, "Check Resize Operation Done\n");
    return 0;
}





