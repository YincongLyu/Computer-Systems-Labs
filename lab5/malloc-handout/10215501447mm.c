#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
//基本宏定义
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4             //字的大小（WSIZE)
#define DSIZE 8             //双字的大小（DSIZE)
#define CHUNKSIZE (1<<12)   //初始空闲块的大小和扩展堆时的默认大小
#define MAX(x, y) ((x)>(y)?(x):(y))
#define PACK(size, alloc) ((size) | (alloc))        //将大小和已分配位结合起来并返回一个值，可以把它存放在头部或者脚部中
#define GET(p) (*(unsigned int *)p)                 //读取和返回参数p引用的字
#define PUT(p, val) (*(unsigned int *)(p) = (val))  //将val存放在参数p指向的字中,写p指向的字
#define GET_SIZE(p) (GET(p) & ~0x7) //从地址p处的头部或者脚部返回块的大小
#define GET_ALLOC(p) (GET(p) & 0x1) //从地址p处的头部或者脚部获得块是否已分配
#define HDRP(bp) ((char *)(bp) - WSIZE)     //返回指向bp这个块的头部的指针
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)    //返回指向bp这个块的脚部的指针
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) //返回bp下一个块的块指针
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //返回bp上一个块的块指针

#define MINBLOCKSIZE 16
#define GETPREV(bp) (*(void**)(bp))
#define SETPREV(bp, addr) (*(void**)(bp) = (addr))
#define GETNEXT(bp) (*(void**)((bp) + sizeof(void*)))
#define SETNEXT(bp, addr) (*(void**)((bp) + sizeof(void*)) = (addr))

#define listnum 16
static char* heap_listp;
static char* separated_free_lists[listnum];

static void* extend_heap(size_t words);
static void* coalesce(void* bp);
static void* find_fit(size_t asize);
static void* place(void* bp, size_t asize);
static void insert_list(void* bp);
static void delete_list(void* bp);
int mm_init(void);
void* mm_malloc(size_t size);
void mm_free(void* bp);
void* mm_realloc(void* ptr, size_t size);

team_t team = {
    /* Team name */
    "zhangyang",
    /* First member's full name */
    "zhangyang",
    /* First member's email address */
    "1549723154@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

static void insert_list(void* bp) {
    int index = 0;
    char* search_ptr = NULL;
    char* insert_ptr = NULL;
    size_t size = GET_SIZE(HDRP(bp));

    while ((index < listnum - 1) && (size > 1)) {
        size >>= 1;
        index++;
    }
    search_ptr = separated_free_lists[index];
    while ((search_ptr != NULL) && (size > GET_SIZE(HDRP(search_ptr)))) {
        insert_ptr = search_ptr;
        search_ptr = (char*)GETNEXT(search_ptr);
    }

    /* insert node */
    if (search_ptr != NULL) {
        /* insert between insert_ptr and search_ptr */
        SETNEXT(bp, search_ptr);
        SETPREV(bp, insert_ptr);
        SETNEXT(insert_ptr, bp);
        SETPREV(search_ptr, bp);
    } else {
        /* insert at the end */
        SETNEXT(bp, NULL);
        SETPREV(bp, insert_ptr);
        if (insert_ptr != NULL) {
            SETNEXT(insert_ptr, bp);
        } else {
            /* insert at the head */
            separated_free_lists[index] = bp;
        }
    }
}

static void delete_list(void* bp) {
    int index = 0;
    size_t size = GET_SIZE(HDRP(bp));
    while ((index < listnum - 1) && (size > 1)) {
        size >>= 1;
        index++;
    }
    char* prev_ptr = GETPREV(bp);
    char* next_ptr = GETNEXT(bp);
    if (prev_ptr) {
        SETNEXT(prev_ptr, next_ptr);
    } else {
        separated_free_lists[index] = next_ptr;
    }
    if (next_ptr) {
        SETPREV(next_ptr, prev_ptr);
    }
}

static void* extend_heap(size_t words) {
    char* bp;
    size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1) return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp);
}

static void* coalesce(void* bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    void* prev_bp = PREV_BLKP(bp);
    void* next_bp = NEXT_BLKP(bp);

    if (!prev_alloc) {
        delete_list(prev_bp);
        size += GET_SIZE(HDRP(prev_bp));
        bp = prev_bp;
    }

    if (!next_alloc) {
        delete_list(next_bp);
        size += GET_SIZE(HDRP(next_bp));
    }

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));

    insert_list(bp);

    return bp;
}

static void* find_fit(size_t asize) {
int size = asize;
int index = 0;
void* ptr = NULL;
void* closest_ptr = NULL;
size_t closest_size = 0;
while (index < listnum) {
    if ((asize <= 1) && (separated_free_lists[index] != NULL)) {
        ptr = separated_free_lists[index];
        while (ptr != NULL) {
            if (size <= GET_SIZE(HDRP(ptr)))
            {
                return ptr;
            } 
            else if (GET_SIZE(HDRP(ptr)) < closest_size)
            {
                closest_ptr = ptr;
                closest_size = GET_SIZE(HDRP(ptr));
            }
            ptr = (void*)GETNEXT(ptr);
        }
    }
    asize >>= 1;
	index++;
}
    return closest_ptr;
}

static void* place(void* bp, size_t asize) {
	size_t csize = GET_SIZE(HDRP(bp));
	delete_list(bp);
	if ((csize - asize) < MINBLOCKSIZE) {
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
		return bp;
	} else if (asize >= 64) {
		PUT(HDRP(bp), PACK(csize - asize, 0));
		PUT(FTRP(bp), PACK(csize - asize, 0));
		PUT(HDRP(NEXT_BLKP(bp)), PACK(asize, 1));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(asize, 1));
		insert_list(bp);
		return NEXT_BLKP(bp);
	} else {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		void* next_ptr = NEXT_BLKP(bp);
		PUT(HDRP(next_ptr), PACK(csize - asize, 0));
		PUT(FTRP(next_ptr), PACK(csize - asize, 0));
		insert_list(next_ptr);
		return bp;
	}
}

int mm_init(void) {
	int index;
	for (index = 0; index < listnum; index++) {
		separated_free_lists[index] = NULL;
	}

	if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void*)-1) return -1;
	PUT(heap_listp, 0);
	PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
	heap_listp += (2 * WSIZE);

	if (extend_heap(CHUNKSIZE / WSIZE) == NULL) return -1;
	return 0;
}

void* mm_malloc(size_t size) {
	size_t asize;
	size_t extend_size;
	char* bp;

	if (size == 0) return NULL;
	if (size <= DSIZE)asize = 2 * DSIZE;
	else asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

	if ((bp = find_fit(asize)) != NULL) {
		return place(bp, asize);
	}

	extend_size = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extend_size / WSIZE)) == NULL) return NULL;
	return place(bp, asize);
}

void mm_free(void* bp) {
	size_t size = GET_SIZE(HDRP(bp));
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp);
}

void* mm_realloc(void* ptr, size_t size) {
	void* new_ptr = ptr;
	int remainder;

	if (size == 0) return NULL;
	if (size <= DSIZE) 
    {
		size = 2 * DSIZE;
	} 
    else 
    {
		size = ALIGN(size + DSIZE);
	}
	if ((remainder = GET_SIZE(HDRP(ptr)) - size) >= 0) {
		return ptr;
	} 
    else if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) || !GET_SIZE(HDRP(NEXT_BLKP(ptr)))) 
    {
        if ((remainder = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - size) < 0) 
        {
            if (extend_heap(MAX(-remainder, CHUNKSIZE) / WSIZE) == NULL) 
            {
                return NULL;
            }
		remainder += MAX(-remainder, CHUNKSIZE);
        }
		delete_list(NEXT_BLKP(ptr));
		PUT(HDRP(ptr), PACK(size + remainder, 1));
		PUT(FTRP(ptr), PACK(size + remainder, 1));
	} 
    else 
    {
		new_ptr = mm_malloc(size);
		memcpy(new_ptr, ptr, GET_SIZE(HDRP(ptr)));
		mm_free(ptr);
	}
	return new_ptr;
}
