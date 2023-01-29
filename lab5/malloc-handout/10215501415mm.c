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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
/* single word (4) or double word (8) alignment */

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

#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 8
#define DSIZE 16
#define nil 0
#define CHUNK_SIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// header开头存放chunk大小，最后1位存放是否分配，use为分配状态，用或运算改变size的最后一位，存放分配状态
#define PACT(size, use) ((size) | (use))

#define chunk_index(p, index) ((p) + index) // index为偏移量，指针移动到对应位置

#define get(p, index) (*(unsigned int*)chunk_index(p, index)) // 获取p+index位置指针中内容
#define put(p, index, val) (get(p, index) = (val)) // 将val放入p+index位置上

// 把上面的get和put封装一下
#define GET(p) (get(p, 0))
#define PUT(p, val) (put(p, 0, val))

#define GETSIZE(p) (GET(p) & ~0x7) // 取出header中存放内容，将最后3位的0恢复（与~7&，只保留前面的1，清空最后3位），得到chunk大小
#define isuse(p) (GET(p) & 0x1) // 取出header中存放内容，只要最后一位当前chunk分配状态
#define prev_isuse(p) ((GET(p) & 0x2) >> 1) // header中倒数第二位为前一个chunk的分配状态，取出来

#define NEXT(p) ((char*)(p) + GETSIZE((p))) // 指向后一个chunk
#define PREV(p) ((char*)(p) - GETSIZE((p) - WSIZE)) // 指向前一个chunk

#define CHUNK_INUSE 1
#define PREV_INUSE 2

// 设置当前chunk状态为已分配，改变当前header的分配状态，同时将下一个chunk的header中上一个chunk状态改为已分配
#define set_inuse(p, size)                                                   \
{                                                                            \
  put(p, 0, (size | 1 | (prev_isuse(p) << 1)));                              \
  put(p, size, (GET(NEXT(p)) | PREV_INUSE));                                 \
}

// p指向大小为size的chunk，把它移出空闲块链表，状态设置为已分配
#define inuse(p, size)                                                       \
{                                                                            \
  unlink(p);                                                                 \
  set_inuse(p, size);                                                        \
}

// 设置当前chunk状态为未分配，同时设置下一个chunk中上一个chunk状态为未分配
#define set_unuse(p, size)                                                   \
{                                                                            \
  put(p, 0, (size | (prev_isuse(p) << 1)));                                  \
  put(p, size - WSIZE, size);                                                \
  put(p, size, (GET(NEXT(p)) & (~PREV_INUSE)));                              \
}

// p指向大小为size的chunk，把它加入空闲块链表，状态设置为未分配
#define unuse(p, size)                                                       \
{                                                                            \
  link(p);                                                                   \
  set_unuse(p, size);                                                        \
}

#define FD(p) (*((unsigned long*)chunk_index(p, WSIZE))) // 取出指向前一个chunk的指针的内容,foreward
#define BK(p) (*((unsigned long*)chunk_index(p, 2 * WSIZE))) // 取出指向后一个chunk的指针内容,backward

// 将p指向的chunk连到heap_list后面
#define link(p)                                                              \
{                                                                            \
  BK(p) = BK(heap_list);                                                     \
  FD(p) = (unsigned long)heap_list;                                          \
  BK(heap_list) = (unsigned long)p;                                          \
  FD(BK(p)) = (unsigned long)p;                                              \
}

// 将p指向的chunk从heap_list后面移出去
#define unlink(p)                                                            \
{                                                                            \
  BK(FD(p)) = BK(p);                                                         \
  FD(BK(p)) = FD(p);                                                         \
  FD(p) = nil;                                                               \
  BK(p) = nil;                                                               \
}

#define chunk2mem(p) (chunk_index(p, WSIZE)) // p指向header，向后移动到payload开始的地方
#define mem2chunk(mem) (chunk_index(mem, -WSIZE)) // mem指向payload，向前移动到header开始的地方

/*#define DEBUG*/

#ifdef DEBUG
#define DPRINT(...) printf(__VA_ARGS__)
#define CHECK() chunk_check()
#else
#define DPRINT(...)
#define CHECK()
#endif

void* heap_list; // 空白chunk的链表

static void* extend_heap(int size);
static void* coalesced(void *p);
static void place(void *p, size_t size);
static void* find_fit(size_t size);
static void* realloc_coalesced(void *tmp, size_t newsize, size_t oldsize);
static void* realloc_place(void *tmp, size_t newsize, size_t oldsize);

// 向某个链表中扩展一个新的大小的块（空的）进去
static void* extend_heap(int word)
{
  void *bp; // 用来指向待加入的新扩展的空间（块）
  size_t size = (word % 2) ? ((word + 1) * WSIZE) : (word * WSIZE); // 按双数大小字节扩展，这样方便对齐

  // mem_sbrk(size)：将堆扩展size字节大小，返回指向堆顶的指针
  if((bp = mem_sbrk(size)) == (void *)-1){
    return NULL;
  }

  unuse(bp, size); // 设置新分配出来的bp空间为未使用，加入空闲块链表
  PUT((NEXT(bp)), (CHUNK_INUSE)); // // 设置一个新的结尾块（状态为已分配）
  return coalesced(bp); // 检查前后有没有可合并的chunk
}

// 检查前后是否有可合并的块
static void* coalesced(void* p)
{
  size_t prev_use = prev_isuse(p); // 前一个chunk的使用状态
  size_t next_use = isuse((NEXT(p))); // 后一个chunk的使用状态
  size_t size = GETSIZE((p)); // 当前chunk的大小

  // 前后都已使用，直接返回
  if (prev_use && next_use){
    return p;
  }
  // 前面已使用，后面有空闲，合并之后修改大小，重新加入链表
  else if (prev_use && (!next_use)){
    unlink(NEXT(p));
    unlink(p);
    size += GETSIZE((NEXT(p)));
    unuse(p, size);
  }
  // 前面未使用，后面已使用，合并后修改大小，将指针移到最前，重新加入链表
  else if ((!prev_use) && next_use){
    unlink(PREV(p));
    unlink(p);
    size += GETSIZE((PREV(p)));
    p = PREV(p);
    unuse(p, size);
  }
  // 前后均为空，合并后修改大小，将指针移到最前，重新加入链表
  else if ((!prev_use) && (!next_use)){
    unlink(PREV(p));
    unlink(NEXT(p));
    unlink(p);
    size += GETSIZE((PREV(p))) + GETSIZE((NEXT(p)));
    p = PREV(p);
    unuse(p, size);
  }
  // 返回指向这个新的空闲块的payload开头的指针
  return p;
}

// 需要size大小的chunk来分配，如果chunk大小比size大了4个WSIZE出来（一个chunk的最小大小），就进行切割后分配；否则直接分配
static void place(void *p, size_t size) {
  size_t psize = GETSIZE((p)); // 当前chunk大小

  // 若差值不在双字对齐范围之内，要把过大的部分切掉变成小块放回去
  if((psize - size) >= (2 * DSIZE)) {
    inuse(p, size); // 把size大小的块切出去，设置为已分配
    unuse(NEXT(p), (psize - size)); // 剩下的放回去，设置为空闲
  }
  // 差值在双字范围内，不用改
  else {
    inuse(p, psize);
  }
}

// 寻找合适大小的块
static void* find_fit(size_t size)
{
  void* tmp = (void*)BK(heap_list); // 从链表的第一个chunk开始遍历

  while(tmp != heap_list){
    if (GETSIZE(tmp) >= (unsigned int)(size)){
      return tmp; // 找到就返回
    }
    tmp = (void*)BK(tmp); // 没找到继续向后遍历
  }
  return NULL;
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  // 分配四个初始的块，一个开头，两个序言，一个结尾，双向链表表头
  // heap_list最开始永远指向第一个对齐的开头块
  if ((heap_list = mem_sbrk(4 * WSIZE)) == (void *)-1){
    return -1;
  }

  BK(heap_list) = FD(heap_list) = (size_t)heap_list;
  inuse(heap_list, 4 * WSIZE); // 最开始四个块表示边界，都设置为已分配状态

  BK(heap_list) = FD(heap_list) = (size_t)heap_list;

  PUT(NEXT(heap_list), (CHUNK_INUSE | PREV_INUSE)); // 序言块设置为已分配

  if (extend_heap(CHUNK_SIZE / WSIZE) == NULL){
    return -1;
  }
  return 0;
}

void* mm_malloc(size_t size)
{
  int newsize;
  void* tmp;

  if(heap_list == NULL) {
    mm_init();
  }

  // 判断请求是否有效，大小为0直接退出
  if(size == 0) {
    return NULL;
  }

  // 块小于双字对齐，直接分配双字大小
  if(size <= DSIZE){
    newsize = 2 * DSIZE;
  }
  else{
    newsize = WSIZE * ((size + (DSIZE) + (WSIZE - 1)) / WSIZE);
  }

  // 如果在堆中可以找到合适的空闲块，直接放
  if((tmp = find_fit(newsize)) != NULL){
    place(tmp, newsize);
    DPRINT("malloc(0x%x) [%p]\n", newsize, tmp);
    CHECK();
    return chunk2mem(tmp); // 转化为指向payload的指针返回
  }

  // 找不到就要扩展堆，先计算所需大小
  size_t extend_size = MAX(newsize, CHUNK_SIZE);

  if((tmp = extend_heap(extend_size / WSIZE)) == NULL){
    DPRINT("malloc(0x%x) [%p]\n", newsize, tmp);
    CHECK();
    return NULL; // 扩展不成功
  }
  
  place(tmp, newsize); // tmp指向新分配好的chunk

  DPRINT("malloc(0x%x) [%p]\n", newsize, tmp);
  CHECK();

  return chunk2mem(tmp); // 返回payload开头的指针
}

void mm_free(void* ptr)
{
  if(ptr == 0){
    return;
  }
  void* tmp = mem2chunk(ptr); // tmp从payload指向header
  size_t size = GETSIZE((tmp)); // 获取当前chunk大小
  unuse(tmp, size); // 释放
  coalesced(tmp); // 检查前后是否能合并

  DPRINT("free [%p]\n", tmp);
  CHECK();
}

void* mm_realloc(void *ptr, size_t size)
{
  DPRINT("\nrealloc [%p](0x%x) -> (0x%x)\n", mem2chunk(ptr),GETSIZE(mem2chunk(ptr)), size);

  size_t oldsize, newsize;
  void* tmp, *p, *newptr;

  // 当前为空，直接分配
  if (ptr == NULL) {
    p = mm_malloc(size);
    return p;
  }

  // size = 0相当于free
  if (size == 0) {
    mm_free(ptr);
    return NULL;
  }

  // tmp指向要操作的chunk header
  tmp = mem2chunk(ptr);
  oldsize = GETSIZE(tmp); // 原来的大小
  // newsize = size + WSIZE;

  if(size <= DSIZE){
    newsize = 2 * DSIZE; // 最小要求双字对齐
  }
  else{
    newsize = WSIZE * ((size + (DSIZE) + (WSIZE - 1)) / WSIZE);
  }

  // 老的大小大于新的所需要的大小，直接重新分配大小即可，payload中内容不用动
  if(oldsize > newsize){
    p = realloc_place(tmp, newsize, oldsize);
    newptr = chunk2mem(p); // 返回指向payload指针
    DPRINT("place [0x%x] -> [0x%x]\n", oldsize, newsize);
    return newptr;
  }
  // 老的大小小于新的所需要的大小，需要空白chunk合并进来
  else if(oldsize < newsize){
    p = realloc_coalesced(tmp, newsize, oldsize); // 看前后有没有能合并进来变成需要大小的空白块

    // 如果前后找不到合并后的空白块可满足新的大小，重新分配，并把payload里内容移动过去
    if(p == NULL){
      newptr = mm_malloc(newsize);
      memcpy(newptr, ptr, oldsize);
      mm_free(ptr); // 把老的块free掉
      DPRINT("re malloc \n");
      return newptr;
    }
    else{
      newptr = chunk2mem(p); // 新合并好的块的payload开始位置
      // 如果新合并的开头和老的开头是一样的，直接返回
      if(newptr == ptr){
        DPRINT("coalesced next\n");
        return newptr;
      }
      // 不一样要把payload里内容往前挪，然后返回新的开头
      else{
        memcpy(newptr, ptr, oldsize - WSIZE);
        realloc_place(p, newsize, GETSIZE(p));
        DPRINT("coalesced prev\n");
        return newptr;
      }
    }
  }
  // oldsize == newsize，直接返回即可
  DPRINT("return ptr \n");
  return ptr;
}

// 
static void* realloc_coalesced(void *tmp, size_t newsize, size_t oldsize)
{
  void* p;

  size_t prev_use = prev_isuse(tmp); // 前一个chunk的使用状态
  size_t next_use = isuse((NEXT(tmp))); // 后一个chunk的使用状态
  size_t size = oldsize;

  // 前后的chunk均被使用，不合并
  if(prev_use && next_use){
    return NULL;
  }
  // 后一个chunk没被用，前一个被用了
  else if((!next_use) && prev_use){
    size += GETSIZE((NEXT(tmp))); // 合并后的大小

    // 若合并后的有效大小大于新分配的大小，就要修改空闲块大小，重新加入链表
    if(size >= newsize){
      unlink(NEXT(tmp)); // 把当前块从链表中移出去
      set_inuse(tmp, size); // 移出去之后设置为已分配
      return realloc_place(tmp, newsize, size);
    }
  }
  // 前一个chunk没有被使用，后一个被用了
  else if ((!prev_use) && next_use){
    size += GETSIZE((PREV(tmp)));
    if (size >= newsize){
      unlink(PREV(tmp));
      set_inuse(PREV(tmp), size);
      return PREV(tmp);
    }
  }
  // 前后都没被用，全合并
  else if((!prev_use) && (!next_use)){
    size += GETSIZE((PREV(tmp))) + GETSIZE((NEXT(tmp)));
    if(size >= newsize){
      unlink(PREV(tmp));
      unlink(NEXT(tmp));
      set_inuse(PREV(tmp), size);
      return PREV(tmp);
    }
  }
}

// 设置新的块分配状态
static void *realloc_place(void *tmp, size_t newsize, size_t oldsize)
{
  set_inuse(tmp, oldsize);
  return tmp;
}