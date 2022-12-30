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
    "team",
    /* First member's full name */
    "GentleCold",
    /* First member's email address */
    "1952173800@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

// My define
#define WSIZE 4
#define DSIZE 8
#define FSIZE 16
#define CHUNK 1 << 10

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PARSE(v) ((v) & ~0x7)
#define PACK(v, a) ((v) | (a))

#define HEAD(bp) ((byte *)(bp) - WSIZE)
#define FOOT(bp) ((byte *)(bp) + SIZE(bp))
#define SIZE(bp) (PARSE(GET(HEAD(bp))))
#define ALLOC(bp) (GET(HEAD(bp)) & 0x1)

#define GET(p) (*(word *)(p))
#define SET(p, v) (*(word *)(p) = (v))

#define NEXT(bp) (FOOT(bp) + DSIZE)
#define PREV(bp) ((byte *)(bp) - PARSE(GET((bp) - DSIZE)) - DSIZE)

typedef unsigned int word;
typedef char byte;

// mark the front and tail pos
void *front_p = NULL;
void *tail_p = NULL;
// used for next fit, updated by mm_init, mm_malloc, _coalesce
void *fitted_p = NULL;

// My func
/**
 * add a blank chunk and coalesce
 * will update tail_p
 * @param size align by 8, excluding head and foot
 * @return new bp
 */
static void *_extend(size_t size);

/**
 * coalesce blank chunk before and after bp
 * @param bp loaded point
 * @return bp after coalesce
 */
static void *_coalesce(void *bp);
static void *__coalesce_prev(void *bp);
static void *__coalesce_next(void *bp);
static void *__coalesce_all(void *bp);

/**
 * traverse and find first fit, then place in
 * @deprecated too slow
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_first_fit(size_t size);

/**
 * find next fit, then place in
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_next_fit(size_t size);

/**
 * find next fit, then place in, if from beginning, use best fit
 * @deprecated thru loss
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_next_best_fit(size_t size);

/**
 * allocate the block and cut sometimes
 * @param size align by 8, excluding head and foot
 */
static void _place(void *ptr, size_t size);
// end

/**
 * initialize the malloc package.
 * get a new chunk, set front_p and tail_p
 */
int mm_init(void) {
    if ((front_p = mem_sbrk(WSIZE)) == (void *) - 1) return -1; // blank
    front_p += DSIZE; // first chunk
    fitted_p = front_p; // init fitted_p
    if (!_extend(CHUNK)) return -1;
    return 0;
}

/**
 * find first fit or extend
 */
void *mm_malloc(size_t size) {
    size_t adjust_size = ALIGN(size);
    size_t extend_size;
    void *bp;
    if ((bp = _next_fit(adjust_size)) != NULL) {
        fitted_p = bp;
        return bp;
    } else {
        extend_size = adjust_size;
        if (!ALLOC(tail_p)) {
            extend_size -= (SIZE(tail_p) + DSIZE);
        }
        bp = _extend(MAX(extend_size, CHUNK));
        if (bp == NULL) return bp;
        _place(bp, adjust_size);
        fitted_p = bp;
        return bp;
    }
}

/**
 * free a block and coalesce immediately
 */
void mm_free(void *ptr) {
    size_t size = SIZE(ptr);
    SET(HEAD(ptr), PACK(size, 0));
    SET(FOOT(ptr), PACK(size, 0));
    _coalesce(ptr);
}

/**
 * implemented simply in terms of mm_malloc and mm_free
 * compare adjust_size and total_size step by step
 */
void *mm_realloc(void *ptr, size_t size) {
    if (ptr == NULL) return mm_malloc(size);
    if (size == 0) return NULL;

    void *new_ptr;
    size_t adjust_size = ALIGN(size);
    size_t old_size = SIZE(ptr);
    if (adjust_size <= old_size) {
        // just return, for the memory lost is little
        return ptr;
    }
    size_t next_size = (ptr != tail_p && !ALLOC(NEXT(ptr))) ? SIZE(NEXT(ptr)) + DSIZE : 0;
    size_t total_size = old_size + next_size;
    if (adjust_size <= total_size) {
        __coalesce_next(ptr);
        _place(ptr, adjust_size); // just cut
        return ptr;
    }
    size_t prev_size = (ptr != front_p && !ALLOC(PREV(ptr))) ? SIZE(PREV(ptr)) + DSIZE : 0;
    total_size += prev_size;
    if (adjust_size <= total_size) { // coalesce prev or all
        new_ptr = _coalesce(ptr);
        memmove(new_ptr, ptr, old_size);
        _place(new_ptr, adjust_size);
    } else {
        if ((new_ptr = mm_malloc(size)) == NULL) return NULL;
        memmove(new_ptr, ptr, old_size);
        mm_free(ptr);
    }
    return new_ptr;
}

// my func
static void *_extend(size_t size) {
    void *bp;
    if ((bp = mem_sbrk(size + DSIZE)) == (void *) - 1) return NULL;
    // init chunk
    SET(bp, PACK(size, 0));
    bp += WSIZE;
    SET(FOOT(bp), PACK(size, 0));
    // init mark point
    tail_p = bp;
    return _coalesce(bp);
}

static void *_coalesce(void *bp) {
    // one chunk
    if (bp == front_p && bp == tail_p) return bp;
    if (bp == front_p || ALLOC(PREV(bp))) {
        if (bp == tail_p || ALLOC(NEXT(bp))) return bp;
        return __coalesce_next(bp);
    } else if (bp == tail_p || ALLOC(NEXT(bp))) {
        return __coalesce_prev(bp);
    } else {
        return __coalesce_all(bp);
    }
}

static void *__coalesce_prev(void *bp) {
    void *prev = PREV(bp);
    size_t new_size = SIZE(prev) + SIZE(bp) + DSIZE;
    SET(HEAD(prev), PACK(new_size, 0));
    SET(FOOT(bp), PACK(new_size, 0));
    if (bp == tail_p) tail_p = prev;
    if (bp == fitted_p) fitted_p = prev;
    return prev;
}

static void *__coalesce_next(void *bp) {
    void *next = NEXT(bp);
    size_t new_size = SIZE(next) + SIZE(bp) + DSIZE;
    SET(HEAD(bp), PACK(new_size, 0));
    SET(FOOT(next), PACK(new_size, 0));
    if (next == tail_p) tail_p = bp; // should also change
    if (next == fitted_p) fitted_p = bp;
    return bp;
}

static void *__coalesce_all(void *bp) {
    void *prev = PREV(bp);
    void *next = NEXT(bp);
    size_t new_size = SIZE(prev) + SIZE(bp) + SIZE(next) + FSIZE;
    SET(HEAD(prev), PACK(new_size, 0));
    SET(FOOT(next), PACK(new_size, 0));
    if (next == tail_p) tail_p = prev;
    if (next == fitted_p || bp == fitted_p) fitted_p = prev;
    return prev;
}

static void *_first_fit(size_t size) {
    void *bp = front_p;
    void *after_p = NEXT(tail_p);
    while (bp != after_p) {
        if (!ALLOC(bp) && SIZE(bp) >= size) {
            _place(bp, size);
            return bp;
        }
        bp = NEXT(bp);
    }
    return NULL;
}

static void *_next_fit(size_t size) {
    void *bp = fitted_p;
    void *after_p = NEXT(tail_p);
    while (bp != after_p) {
        if (!ALLOC(bp) && SIZE(bp) >= size) {
            _place(bp, size);
            return bp;
        }
        bp = NEXT(bp);
    }
    bp = front_p;
    while (bp != fitted_p) {
        if (!ALLOC(bp) && SIZE(bp) >= size) {
            _place(bp, size);
            return bp;
        }
        bp = NEXT(bp);
    }
    return NULL;
}

static void *_next_best_fit(size_t size) {
    void *bp = fitted_p;
    void *after_p = NEXT(tail_p);
    while (bp != after_p) {
        if (!ALLOC(bp) && SIZE(bp) >= size) {
            _place(bp, size);
            return bp;
        }
        bp = NEXT(bp);
    }
    bp = front_p;
    size_t min = 0;
    void *min_p = NULL;
    while (bp != fitted_p) {
        if (!ALLOC(bp) && SIZE(bp) >= size) {
            if (min_p == NULL || SIZE(bp) < min) {
                min = SIZE(bp);
                min_p = bp;
            }
        }
        bp = NEXT(bp);
    }
    if (min_p == NULL) return NULL;
    _place(min_p, size);
    return min_p;
}

static void _place(void *ptr, size_t size) {
    size_t p_size = SIZE(ptr);
    if (p_size - size >= FSIZE) {
        SET(HEAD(ptr), PACK(size, 1));
        SET(FOOT(ptr), PACK(size, 1));
        // DSIZE adjust
        size_t adjust_size = p_size - size - DSIZE;
        SET(HEAD(NEXT(ptr)), PACK(adjust_size, 0));
        SET(FOOT(NEXT(ptr)), PACK(adjust_size, 0));
        if (ptr == tail_p) tail_p = NEXT(ptr);
    } else {
        SET(HEAD(ptr), PACK(p_size, 1));
        SET(FOOT(ptr), PACK(p_size, 1));
    }
}
