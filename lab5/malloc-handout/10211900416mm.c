//版本三
//98 of 100
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4
#define DSIZE 8
#define INITSIZE (1 << 6)
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */
#define MINBLOCKSIZE 2 * DSIZE

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PACK(size, alloc) \
  ((size) | (alloc)) /* Pack a size and allocated bit into a word */

#define GET(p) (*(unsigned int*)(p)) /* read a word at address p */
#define PUT(p, val) \
  (*(unsigned int*)(p) = (val)) /* write a word at address p */

#define GET_SIZE(p) (GET(p) & ~0x7) /* read the size field from address p */
#define GET_ALLOC(p) (GET(p) & 0x1) /* read the alloc field from address p */

/* given block ptr bp, compute address of its header */
#define HDRP(bp) ((char*)(bp)-WSIZE)
/* given block ptr bp, compute address of its footer */
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* given block ptr bp, compute address of next blocks */
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)))
/* given block ptr bp, compute address of prev blocks */
#define PREV_BLKP(bp) ((char*)(bp)-GET_SIZE((char*)(bp)-DSIZE))

/*useful macro definition related to explicit free lists*/
/* next pointer and pre pointer both take up 32 bits */
#define GET_PREV(bp) (*(unsigned int*)(bp))
#define SET_PREV(bp, addr) (*(unsigned int*)(bp) = (addr))
#define GET_NEXT(bp) (*((unsigned int*)(bp) + 1))
#define SET_NEXT(bp, addr) (*((unsigned int*)(bp) + 1) = (addr))

#define LISTCOUNT 16
static char* heap_listp;
static char* separated_free_lists[LISTCOUNT];

static void* extend_heap(size_t words);
static void* coalesce(void* bp);
static void* find_fit(size_t asize);
static void* place(void* bp, size_t asize);
static void insert_node(void* bp);
static void delete_node(void* bp);
int mm_init(void);
void* mm_malloc(size_t size);
void mm_free(void* bp);
void* mm_realloc(void* ptr, size_t size);

static void insert_node(void* bp) {
	int index = 0;
	char* search_ptr = NULL;
	char* insert_ptr = NULL;
	size_t size = GET_SIZE(HDRP(bp));

	while ((index < LISTCOUNT - 1) && (size > 1)) {
		size >>= 1;
		index++;
	}
	search_ptr = separated_free_lists[index];
	while ((search_ptr != NULL) && (size > GET_SIZE(HDRP(search_ptr)))) {
		insert_ptr = search_ptr;
		search_ptr = (char*)GET_NEXT(search_ptr);
	}
	if (search_ptr != NULL) {
		if (insert_ptr != NULL) {
			char* prev_ptr = GET_PREV(search_ptr);
			SET_NEXT(bp, search_ptr);
			SET_PREV(search_ptr, bp);
			SET_NEXT(prev_ptr, bp);
			SET_PREV(bp, prev_ptr);
		} else {
			SET_NEXT(bp, search_ptr);
			SET_PREV(search_ptr, bp);
			SET_PREV(bp, 0);
			separated_free_lists[index] = bp;
		}
	} else {
		if (insert_ptr != NULL) {
			SET_PREV(bp, search_ptr);
			SET_NEXT(search_ptr, bp);
			SET_NEXT(bp, 0);
		} else {
			SET_NEXT(bp, 0);
			SET_PREV(bp, 0);
			separated_free_lists[index] = bp;
		}
	}
}

static void delete_node(void* bp) {
	int index = 0;
	size_t size = GET_SIZE(HDRP(bp));
	while ((index < LISTCOUNT - 1) && (size > 1)) {
		size >>= 1;
		index++;
	}
	char* prev_ptr = GET_PREV(bp);
	char* next_ptr = GET_NEXT(bp);
	if (prev_ptr) {
		if (next_ptr) {
			SET_NEXT(prev_ptr, next_ptr);
			SET_PREV(next_ptr, prev_ptr);
		} else {
			SET_NEXT(prev_ptr, 0);
		}
	} else {
		if (next_ptr) {
			SET_PREV(next_ptr, 0);
			separated_free_lists[index] = next_ptr;
		} else {
			SET_NEXT(bp, 0);
			SET_PREV(bp, 0);
			separated_free_lists[index] = NULL;
		}
	}
}

team_t team = {
    /* Team name */
    "Tommy",
    /* First member's full name */
    "Tommy",
    /* First member's email address */
    "10211900416@stu.ecnu.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/*
 * extend heap by words * word(4 bytes)
 */
static void* extend_heap(size_t words) {
	char* bp;
	size_t size;

	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1) return NULL;

	/* Initialize free block header/footer and the epilogue header */
	PUT(HDRP(bp), PACK(size, 0));          // free block header
	PUT(FTRP(bp), PACK(size, 0));          // free block footer
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));  // new epilogue header

	return coalesce(bp);  // coalesce if the previous block was free
}

static void* coalesce(void* bp) {
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	void* prev_bp = PREV_BLKP(bp);
	void* next_bp = NEXT_BLKP(bp);

	if (prev_alloc && next_alloc) {
	} else if (prev_alloc && !next_alloc) {
		delete_node(next_bp);
		size += GET_SIZE(HDRP(next_bp));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	} else if (!prev_alloc && next_alloc) {
		delete_node(prev_bp);
		size += GET_SIZE(HDRP(prev_bp));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = prev_bp;
	} else {
		delete_node(next_bp);
		delete_node(prev_bp);
		size += GET_SIZE(HDRP(prev_bp)) + GET_SIZE(FTRP(next_bp));
		PUT(HDRP(prev_bp), PACK(size, 0));
		PUT(FTRP(next_bp), PACK(size, 0));
		bp = prev_bp;
	}
	insert_node(bp);
	return bp;
}

static void* find_fit(size_t asize) {
	int size = asize;
	int index = 0;
	void* ptr = NULL;
	while (index < LISTCOUNT) {
		if ((asize <= 1) && (separated_free_lists[index] != NULL)) {
			ptr = separated_free_lists[index];
			while ((ptr != NULL) && (size > GET_SIZE(HDRP(ptr)))) {
				ptr = (void*)GET_NEXT(ptr);
			}
			if (ptr != NULL) {
				break;
			}
		}

		asize >>= 1;
		index++;
	}
	return ptr;
}

static void* place(void* bp, size_t asize) {
	size_t csize = GET_SIZE(HDRP(bp));
	delete_node(bp);
	if ((csize - asize) < MINBLOCKSIZE) {
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
		return bp;
	} else if (asize >= 96) {
		PUT(HDRP(bp), PACK(csize - asize, 0));
		PUT(FTRP(bp), PACK(csize - asize, 0));
		PUT(HDRP(NEXT_BLKP(bp)), PACK(asize, 1));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(asize, 1));
		insert_node(bp);
		return NEXT_BLKP(bp);
	} else {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		void* next_ptr = NEXT_BLKP(bp);
		PUT(HDRP(next_ptr), PACK(csize - asize, 0));
		PUT(FTRP(next_ptr), PACK(csize - asize, 0));
		insert_node(next_ptr);
		return bp;
	}
}

int mm_init(void) {
	int index;
	for (index = 0; index < LISTCOUNT; index++) {
		separated_free_lists[index] = NULL;
	}

	if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void*)-1) return -1;
	PUT(heap_listp, 0);                             // alignment padding
	PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  // prologue header
	PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  // prologue footer
	PUT(heap_listp + (3 * WSIZE), PACK(0, 1));      // epilogue header
	heap_listp += (2 * WSIZE);

	if (extend_heap(INITSIZE / WSIZE) == NULL) return -1;
	return 0;
}

void* mm_malloc(size_t size) {
	size_t asize;       // adjusted block size
	size_t extendsize;  // amount to extend heap if no fit
	char* bp;

	if (size == 0) return NULL;

	// adjusted block size to include overhead and alignment reqs
	if (size <= DSIZE)asize = 2 * DSIZE;
	else asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

	if ((bp = find_fit(asize)) != NULL) {
		return place(bp, asize);
	}

	// no fit found, get more memory and place the block
	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extendsize / WSIZE)) == NULL) return NULL;
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
	if (size <= DSIZE) {
		size = 2 * DSIZE;
	} else {
		size = ALIGN(size + DSIZE);
	}
	if ((remainder = GET_SIZE(HDRP(ptr)) - size) >= 0) {
		return ptr;
	} else if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) || !GET_SIZE(HDRP(NEXT_BLKP(ptr)))) {
    if ((remainder = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - size) < 0) {
		if (extend_heap(MAX(-remainder, CHUNKSIZE) / WSIZE) == NULL) {
			return NULL;
		}
		remainder += MAX(-remainder, CHUNKSIZE);
    }
		delete_node(NEXT_BLKP(ptr));
		PUT(HDRP(ptr), PACK(size + remainder, 1));
		PUT(FTRP(ptr), PACK(size + remainder, 1));
	} else {
		new_ptr = mm_malloc(size);
		memcpy(new_ptr, ptr, GET_SIZE(HDRP(ptr)));
		mm_free(ptr);
	}
	return new_ptr;
}