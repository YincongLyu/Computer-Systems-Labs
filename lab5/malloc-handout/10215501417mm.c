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
    "李泽朋",
    /* First member's email address */
    "1224060318@qq.com",
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

#define allow_size 16

/* 
 * mm_init - initialize the malloc package.
 */


size_t max(size_t a, size_t b) {
    if (a > b) {
        return a;
    }
    else return b;
}

size_t *find_head_from_ptr(void *ptr) {
    return (size_t *)((char*)ptr - sizeof(size_t));
}

size_t *find_last_foot_from_head(void *ptr) {
    return (size_t *)((char*)ptr - sizeof(size_t));
}

size_t *find_next_head_from_head(void *ptr, char avaliable) {
    size_t bsize;
    if (avaliable) {
        bsize = *(size_t *)ptr - 1;
    }
    else {
        bsize = *(size_t *)ptr;
    }
    return (size_t *)((char*)ptr + bsize);
}

size_t *find_head_from_foot(void *ptr, char avaliable) {
    size_t bsize;
    if (avaliable) {
        bsize = *(size_t *)ptr - 1;
    }
    else {
        bsize = *(size_t *)ptr;
    }
    return (size_t *)((char*)ptr - bsize + sizeof(size_t));
}

size_t *find_foot_from_head(void *ptr, char avaliable) {
    size_t bsize;
    if (avaliable) {
        bsize = *(size_t *)ptr - 1;
    }
    else {
        bsize = *(size_t *)ptr;
    }
    return (size_t *)((char*)ptr + bsize - sizeof(size_t));
}

size_t check_avaliable(size_t* ptr) {
    // 检查是否空闲
    size_t mask = 1;
    return (*ptr) & mask;
}

size_t *start;
size_t *end;
size_t *now_ptr;

int max_free_block_size = 0;
int touch_end = 0;

int freed_block = 0;


int mm_check(void) {
    printf("check -------------------------------- check\n");
    size_t* ptr = start + 1;
    size_t* last_head = NULL;
    printf("start ptr : %p\n", start);
    while(*ptr != 1) {
        if (last_head == ptr) {
            printf("ERROR last_head == ptr\n");
            break;
        }
        size_t ava = check_avaliable(ptr);
        printf("[%p : %5d ----%5d---%d---- %p : %5d]\n", ptr, *ptr, *ptr - check_avaliable(ptr), ava, (char*)find_foot_from_head(ptr, ava) + 3, *find_foot_from_head(ptr, ava));
        last_head = ptr;
        ptr = find_next_head_from_head(ptr, ava);
    }
    printf("end ptr : %p *ptr : %d\n", ptr, *ptr);
    printf("check -------------------------------- check\n");
    return 0;
}

int mm_init(void)
{
    max_free_block_size = 0;
    touch_end = 0;

    freed_block = 0;
    // printf("\n");
    start = mem_sbrk(sizeof(size_t));
    now_ptr = start + 1;
    // printf("init start : %p\n", start);
    end = mem_sbrk(sizeof(size_t));
    // printf("init end : %p\n", end);
    if (start == NULL || end == NULL)
    {
        return -1;
    }
    *end = 1;
    return 0;
}

void* find_next(size_t size) {
    
    // printf("\n*********************************   malloc first\n");
    // mm_check();
    // 寻找空闲链表，将找到的第一个空闲块输出，否则输出NULL

    // 防止free调后合并块，使得now_ptr不再是块首
    size_t* ptr = now_ptr;
    // printf("size : %d\n", size);
    // printf("now_ptr : %p\n", now_ptr);
    // size_t* ptr = start + 1;
    void* ret_ptr = NULL;
    size_t ava;
    while (1) {
        if (*ptr == 1) {
            if (max_free_block_size == -1)
            {
                if (touch_end == 1) {
                    max_free_block_size = 0;
                }
                else if (touch_end == 0){
                    touch_end = 1;
                }
            }
            now_ptr = start + 1;
            break;
        }
        else if ((ava = check_avaliable(ptr)) == 0) {
            if (*ptr >= size) {
                ret_ptr = (void*)ptr;
                now_ptr = find_next_head_from_head(ptr, ava);
                if (*now_ptr == 1) {
                    now_ptr = start + 1;
                }
                break;
            }
            else {
                if (max_free_block_size == -1) {
                    max_free_block_size = *ptr;
                }
                else {
                    max_free_block_size = max(max_free_block_size, *ptr);
                }
            }
        }
        ptr = find_next_head_from_head(ptr, ava);
    }
    // printf("now_ptr : %p\n", now_ptr);
    // printf("ret_ptr : %p   now_ptr : %p\n", ret_ptr, now_ptr);
    // printf("*********************************   malloc first - end\n");
    return ret_ptr;
}

size_t* check_last_is_free() {
    size_t* last_foot = (size_t*)((char*)mem_heap_hi() - 7);
    if (check_avaliable(last_foot) == 0 && last_foot != start)
    {
        return (size_t*)find_head_from_foot(last_foot, 0);
    }
    else {
        return NULL;
    }
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */

void *mm_malloc(size_t size)
{
    // printf("-----------------------------------------------------------------   malloc\n");
    // mm_check();
    // 待做 先检查空闲块，再分配新的
    if (size == 0)
    {
        return NULL;
    }
    int newsize = ALIGN(size + sizeof(size_t)*2);
    // printf("demand size : %d\n", size);
    // printf("max_f_size1 : %d\n", max_free_block_size);
    // 情况标记 0代表有空闲块可以存放，2代表最后一个虽然不够大但是空闲，1代表需要重新申请完整的块
    int is_realloc = 0;
    void* p = NULL;
    // printf("here1\n");
    if (freed_block == 1)
    {
        // printf("here2\n");
        // printf("is_realloc : %d\n", is_realloc);
        if (max_free_block_size == -1 || max_free_block_size >= newsize) {
            // printf("here first\n");
            p = find_next(newsize);
            // printf("max_f_size2 : %d\n", max_free_block_size);
            // printf("m_f : %p\n", p);
        }
        if (p == NULL) {
            // printf("malloc first return NULL\n");
            size_t* ret_head = check_last_is_free();

            if (ret_head != NULL) {
                // printf("here2\n");
                is_realloc = 2;
                // 防止没进入find_next 但是最后一个块空闲且可以放得下
                if (*ret_head > newsize) {
                    is_realloc = 0;
                }
                p = (void*)ret_head;
            }
            else {
                is_realloc = 1;
                p = mem_sbrk(newsize);
                if (p == (void *)-1)
                {
                    // printf("here1\n");
                    return NULL;
                }
            }
        }
    }
    else 
    {
        is_realloc = 1;
        p = mem_sbrk(newsize);
        if (p == (void *)-1) return NULL;
    }
    // printf("is_realloc : %d\n", is_realloc);
    
    void* will_ret_ptr;
    // 新申请的块
    if (is_realloc == 1) {
        // printf("here 1\n");
        size_t* p_head = (size_t*)p - 1;
        size_t* p_foot = (size_t *)((char*)p_head + newsize - sizeof(size_t));
        size_t* p_end = (size_t *)((char*)p_head + newsize);
        // printf("%p %p %p\n", p_head, p_foot, p_end);
        *p_head = newsize + 1;
        *p_foot = newsize + 1;
        *p_end = 1;
        end = p_end;
        will_ret_ptr = (void*)(p_head + 1);
        // printf("after malloc p_head : %p *p_head : %d\n", p_head, *p_head);
        // printf("after malloc p_foot : %p *p_foot : %d\n", p_foot, *p_foot);
    }
    else if (is_realloc == 2) {
        // printf("here 2\n");
        size_t* p_head = (size_t*)p;
        if (max_free_block_size == *p_head) {
            max_free_block_size = -1;
        }
        // demand_size 是用上最后那个空闲块还差多少
        size_t demand_size = newsize - *p_head;
        void* ret = mem_sbrk(demand_size);
        if (ret == (void *)-1) {
            return NULL;
        }
        size_t* p_foot = (size_t *)((char*)p_head + newsize - sizeof(size_t));
        size_t* p_end = (size_t *)((char*)p_head + newsize);
        *p_head = newsize + 1;
        *p_foot = newsize + 1;
        *p_end = 1;
        end = p_end;
        will_ret_ptr = (void*)(p_head + 1);    
    }
    // 利用空闲块重新分配
    else {
        // printf("here 0\n");
        size_t total_size = *(size_t*)p;
        // printf("total_size : %d\n", total_size);
        if (max_free_block_size == total_size) {
            max_free_block_size = -1;
        }
        if (total_size - newsize < allow_size) {
            size_t* p_head = (size_t*)p;
            // printf("p_head : %p\n", p_head);
            size_t* p_foot = find_foot_from_head(p, 0);
            *p_head += 1;
            *p_foot += 1;
            will_ret_ptr = (void*)(p_head + 1);
        }
        else {
            // printf("--in 3--\n");
            size_t* p_head = (size_t*)p;
            // printf("p_head : %p\n", p_head);
            size_t* p_foot = (size_t*)((char*)p + newsize - sizeof(size_t));
            // printf("p_foot : %p\n", p_foot);
            size_t* p_next_head = (size_t*)((char*)p_foot + sizeof(size_t));
            // printf("p_next_head : %p\n", p_next_head);
            size_t* p_next_foot = find_foot_from_head(p, 0);
            // printf("p_next_foot : %p\n", p_next_foot);
            *p_head = newsize + 1;
            *p_foot = newsize + 1;
            // printf("total size : %d\n", total_size);
            *p_next_head = total_size - newsize;
            // printf("*p_next_head : %d\n", *p_next_head);
            *p_next_foot = total_size - newsize;
            will_ret_ptr = (void*)(p_head + 1);
        }
    }
    // printf("malloc return ptr : %p\n", will_ret_ptr);
    // printf("max_f_size3 : %d\n", max_free_block_size);
    // mm_check();
    return will_ret_ptr;
}

// 合并的时候判断开始与结束块有问题

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    // printf("-----------------------------------------------------------------   free\n");
    // mm_check();
    // printf("free ptr : %p\n", ptr);
    size_t* head = (size_t*)ptr - 1;
    if (head <= start || head >= end) return;
    size_t* foot = find_foot_from_head((size_t*)ptr - 1, check_avaliable((size_t*)ptr - 1));
    // printf("head : %p, end : %p\n", ((size_t*)ptr - 1), find_foot_from_head((size_t*)ptr - 1, check_avaliable((size_t*)ptr - 1)));
    // printf("*head : %d, *end : %d\n", *((size_t*)ptr - 1), *find_foot_from_head((size_t*)ptr - 1, check_avaliable((size_t*)ptr - 1)));

    if ((foot >= end) || (foot <= start) || (*head != *foot))
    {
        return;
    }

    freed_block = 1;
    
    size_t *ptr_head = find_head_from_ptr(ptr);
    // 标记，0代表空闲，1代表不空闲
    char avaliable_front = 0;
    char avaliable_back = 0;
    size_t *ptr_front_foot = find_last_foot_from_head(ptr_head);
    size_t *ptr_back_head = find_next_head_from_head(ptr_head, 1);
    // printf("in --0 - 1--\n");
    // printf("ptr_front_foot == start : %d\n", ptr_front_foot == start);
    // printf("ptr_front_foot : %p\n", ptr_front_foot);
    // printf("check_avaliable(ptr_front_foot) : %d\n", check_avaliable(ptr_front_foot));
    if(ptr_front_foot == start || check_avaliable(ptr_front_foot) == 1) {
        avaliable_front = 1;
    }
    if(*ptr_back_head == 1 || check_avaliable(ptr_back_head) == 1) {
        avaliable_back = 1;
    }
    if (avaliable_front == 0 && avaliable_back == 0) {
        // printf("in --1--\n");
        size_t *ptr_front_head = find_head_from_foot(ptr_front_foot, 0);
        // printf("in --1 - 1--\n");
        size_t *ptr_back_foot = find_foot_from_head(ptr_back_head, 0);
        // printf("in --1 - 2--\n");
        size_t bsize = (*ptr_front_head) + (*ptr_back_head) + (*ptr_head) - 1;
        // printf("in --1 - 3--\n");
        *ptr_front_head = bsize;
        // printf("in --1 - 4--\n");
        *ptr_back_foot = bsize;
        // printf("in --1 - 5--\n");
        if (max_free_block_size == -1) {
            max_free_block_size = bsize;
        }
        else {
            max_free_block_size = max(max_free_block_size, bsize);
        }

        // 防止now_ptr指向此合并块的后一部分
        if (now_ptr == ptr_head || now_ptr == ptr_back_head)
        {
            now_ptr = ptr_front_head;
        }
    }
    else if (avaliable_front == 1 && avaliable_back == 0){
        // printf("in --2--\n");
        size_t *ptr_back_foot = find_foot_from_head(ptr_back_head, 0);
        size_t bsize = (*ptr_back_head) + (*ptr_head) - 1;
        *ptr_head = bsize;
        *ptr_back_foot = bsize;
        if (max_free_block_size == -1) {
            max_free_block_size = bsize;
        }
        else {
            max_free_block_size = max(max_free_block_size, bsize);
        }

        // 防止now_ptr指向此合并块的后一部分
        if (now_ptr == ptr_back_head) {
            now_ptr = ptr_head;
        }
    }
    else if (avaliable_front == 0 && avaliable_back == 1){
        // printf("in --3--\n");
        size_t *ptr_front_head = find_head_from_foot(ptr_front_foot, 0);
        size_t bsize = (*ptr_front_head) + (*ptr_head) - 1;
        size_t *ptr_foot = find_foot_from_head(ptr_head, 1);
        *ptr_front_head = bsize;
        *ptr_foot = bsize;
        if (max_free_block_size == -1) {
            max_free_block_size = bsize;
        }
        else {
            max_free_block_size = max(max_free_block_size, bsize);
        }
        // 防止now_ptr指向此合并块的后一部分
        if (now_ptr == ptr_head) {
            now_ptr = ptr_front_head;
        }
    }
    else {
        // printf("in --4--\n");
        // printf("before free p_head : %p *p_head : %d\n", ptr_head, *ptr_head);
        size_t* ptr_foot = find_foot_from_head(ptr_head, 1);
        *ptr_head -= 1;
        *ptr_foot -= 1;
        if (max_free_block_size == -1) {
            max_free_block_size = *ptr_head;
        }
        else {
            max_free_block_size = max(max_free_block_size, *ptr_head);
        }
        // printf("*ptr_head : %d\n", *ptr_head);
        // printf("after free *p_head : %d\n", *ptr_head);
    }
    // printf("after free max size : %d\n", max_free_block_size);
    // mm_check();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    // printf("-----------------------------------------------------------------   realloc\n");
    // // 待修改
    // printf("oldptr : %p   size : %d\n", ptr, size);
    // mm_check();

    int newsize = ALIGN(size + sizeof(size_t)*2);

    if (ptr == NULL) {
        size_t* ptr = mm_malloc(size);
        // mm_check();
        return (void*)ptr;
    }
    else if (size == 0) {
        mm_free(ptr);
        // mm_check();
        return NULL;
    }
    else
    {
        void *oldptr = ptr;
        void *newptr = NULL;
        size_t copySize;

        mm_free(oldptr);
        if (max_free_block_size == -1 || max_free_block_size >= newsize)
            newptr = find_next(newsize);

        // 这样写是因为free后没有管后面块的head，所以可能还会被标记为1，要得到真实的块大小必须减去状态位。
        copySize = *(size_t *)((char *)oldptr - sizeof(size_t)) - 8 - check_avaliable((size_t*)((char *)oldptr - sizeof(size_t)));
        if (size < copySize) copySize = size;

        if (newptr == NULL)
        {
            // printf("\nhere1\n");
            newptr = mem_sbrk(newsize);
            if (newptr == NULL) {
                return NULL;
            }
            memcpy(newptr, oldptr, copySize);
            size_t* head_ptr = find_head_from_ptr(newptr);
            *head_ptr = newsize + 1;
            size_t* foot_ptr = find_foot_from_head(head_ptr, check_avaliable(head_ptr));
            // printf("head : %p, *head : %d , foot : %p\n", head_ptr, *head_ptr, foot_ptr);
            *foot_ptr = newsize + 1;
            end = foot_ptr + 1;
            *end = 1;
            // printf("newptr : %p,  oldptr : %p,  copySize : %d \n", newptr, oldptr, copySize);
        }
        else {
            if (max_free_block_size == *(size_t*)newptr) {
                max_free_block_size = -1;
            }
            // printf("\nhere2\n");
            newptr = (size_t*)newptr + 1;
            // printf("newptr : %p,  oldptr : %p,  copySize : %d \n", newptr, oldptr, copySize);
            memcpy(newptr, oldptr, copySize);

            size_t* newptr_head = find_head_from_ptr(newptr);
            size_t free_block_size = *newptr_head - check_avaliable(newptr_head);
            size_t* newptr_foot = find_foot_from_head(newptr_head, check_avaliable(newptr_head));
            if (free_block_size > newsize + allow_size) {
                size_t* ptr_head = newptr_head;
                *ptr_head = newsize + 1;
                size_t* ptr_foot = find_foot_from_head(ptr_head, check_avaliable(ptr_head));
                *ptr_foot = newsize + 1;
                size_t* next_head = ptr_foot + 1;
                size_t* next_foot = newptr_foot;
                *next_head = free_block_size - newsize;
                *next_foot = free_block_size - newsize;
            }
            else {
                size_t* ptr_head = newptr_head;
                size_t* ptr_foot = newptr_foot;
                *ptr_head += 1;
                *ptr_foot += 1;
            }
        }
        // mm_check();  
        return newptr;
    }
}



