/*
 * mm-naive.c - The clear list(LIFO), best fit, coalesce immediately malloc package.
 */
#include <string.h>

#include "mm.h"
#include "memlib.h"

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

/* single size_t (4) or double size_t (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

// My define
#define WSIZE 4
#define DSIZE 8
#define FSIZE 16
#define ADDRESS (sizeof(size_t))
#define CHUNK 1 << 12
#define MIN_BLOCK (2 * ADDRESS + DSIZE)

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

#define POS_PRED(bp) ((byte *)(bp))
#define POS_SUCC(bp) (((byte *)(bp) + ADDRESS))
#define GET_PRED(bp) (*(size_t *)POS_PRED(bp))
#define GET_SUCC(bp) (*(size_t *)POS_SUCC(bp))

typedef unsigned int word;
typedef char byte;

// mark the front and tail pos
void *front_p = NULL;
void *tail_p = NULL;

/**
 * used for next fit, updated by mm_init, mm_malloc, _coalesce
 * @deprecated useless for clear list
 */
void *fitted_p = NULL;
void *list_p = NULL;

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
static void *__coalesce_none(void *bp);

/**
 * traverse and find first fit, then place in
 * @deprecated too slow
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_first_fit(size_t size);

/**
 * find next fit, then place in
 * @deprecated I'll use clear list
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
 * traverse blank block only and find first fit, then place in
 * @deprecated for the memory loss
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_first_fit_of_clear(size_t size);

/**
 * best fit for clear list
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_best_fit_of_clear(size_t size);

/**
 * allocate the block and cut sometimes
 * @param size align by 8, excluding head and foot
 */
static void _place(void *ptr, size_t size);

/**
 * just replace in of out in list
 * @param in the block that in the list
 * @param out the block that out the list
 */
static void _fix_list(void *in, void *out);

/**
 * check the number of blank list nums and real blank nums and print
 * @deprecated
 */
static void _check();
// end

/**
 * initialize the malloc package.
 * get a new chunk, set front_p and tail_p
 */
int mm_init(void) {
    if ((front_p = mem_sbrk(WSIZE)) == (void *) - 1) return -1; // blank
    front_p += DSIZE; // first chunk
//    fitted_p = front_p; // init fitted_p
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
    if ((bp = _best_fit_of_clear(adjust_size)) != NULL) {
//        fitted_p = bp;
        return bp;
    } else {
        extend_size = adjust_size;
        if (!ALLOC(tail_p)) {
            extend_size -= (SIZE(tail_p) + DSIZE);
        }
        bp = _extend(MAX(extend_size, CHUNK));
        if (bp == NULL) return bp;
        _place(bp, adjust_size);
//        fitted_p = bp;
        return bp;
    }
}

/**
 * free a block and coalesce immediately
 */
void mm_free(void *ptr) {
#ifdef DEBUG
    printf("---free---\n");
    _check();
    printf("----------\n");
#endif
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
        void *next = NEXT(ptr);
        // remove
        if (total_size - adjust_size >= MIN_BLOCK) {
            SET(POS_SUCC(GET_PRED(next)), GET_SUCC(next));
            SET(POS_PRED(GET_SUCC(next)), GET_PRED(next));
            SET(HEAD(ptr), PACK(total_size, 1));
            SET(FOOT(ptr), PACK(total_size, 1));
            if (next == list_p) {
                if (GET_SUCC(next) == (size_t) next) list_p = NULL;
                else list_p = (void *) GET_SUCC(next);
            }
            if (next == tail_p) tail_p = ptr;
        } else { // replace
            void *pred = (void *)GET_PRED(next);
            void *succ = (void *) GET_SUCC(next);
            SET(HEAD(ptr), PACK(adjust_size, 1));
            SET(FOOT(ptr), PACK(adjust_size, 1));
            size_t new_size = total_size - adjust_size - DSIZE;
            void *new = NEXT(ptr);
            SET(HEAD(new), PACK(new_size, 0));
            SET(FOOT(new), PACK(new_size, 0));
            if (pred == next) {
                SET(POS_PRED(new), (size_t)new);
                SET(POS_SUCC(new), (size_t)new);
            } else {
                SET(POS_PRED(succ), (size_t)new);
                SET(POS_SUCC(pred), (size_t)new);
            }
            if (list_p == next) list_p = new;
            if (next == tail_p) tail_p = new;
        }
        return ptr;
    } else {
        if ((new_ptr = mm_malloc(size)) == NULL) return NULL;
        memmove(new_ptr, ptr, old_size);
        mm_free(ptr);
        return new_ptr;
    }
}

// my func
static void *_extend(size_t size) {
    void *bp;
    if ((bp = mem_sbrk(size + DSIZE)) == (void *) - 1) return NULL;
    // init chunk
    SET(bp, PACK(size, 0));
    bp += WSIZE;
    SET(FOOT(bp), PACK(size, 0));
#ifdef DEBUG
    if (tail_p) {
        printf("----extend----\n");
        _check();
        printf("--------------\n");
    }
#endif
    // init mark point
    tail_p = bp;
    return _coalesce(bp);
}

static void *_coalesce(void *bp) {
    // one chunk
    if (bp == front_p && bp == tail_p) return __coalesce_none(bp);
    if (bp == front_p || ALLOC(PREV(bp))) {
        if (bp == tail_p || ALLOC(NEXT(bp))) return __coalesce_none(bp);
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
//    if (bp == fitted_p) fitted_p = prev;
    list_p = prev;
    return prev;
}

static void *__coalesce_next(void *bp) {
    void *next = NEXT(bp);
    // tweak list
    _fix_list(next, bp);
    // coalesce
    size_t new_size = SIZE(next) + SIZE(bp) + DSIZE;
    SET(HEAD(bp), PACK(new_size, 0));
    SET(FOOT(next), PACK(new_size, 0));
    if (next == tail_p) tail_p = bp; // should also change
//    if (next == fitted_p) fitted_p = bp;
    list_p = bp;

#ifdef DEBUG
    printf("----coalesce next----\n");
    _check();
    printf("---------------------\n");
#endif
    return bp;
}

static void *__coalesce_all(void *bp) {
    void *prev = PREV(bp);
    void *next = NEXT(bp);
    // tweak list
    SET(POS_SUCC(GET_PRED(next)), GET_SUCC(next));
    SET(POS_PRED(GET_SUCC(next)), GET_PRED(next));
    // coalesce
    size_t new_size = SIZE(prev) + SIZE(bp) + SIZE(next) + FSIZE;
    SET(HEAD(prev), PACK(new_size, 0));
    SET(FOOT(next), PACK(new_size, 0));
    if (next == tail_p) tail_p = prev;
//    if (next == fitted_p || bp == fitted_p) fitted_p = prev;
    list_p = prev;
#ifdef DEBUG
    printf("---coalesce all---\n");
    _check();
    printf("-------------------\n");
#endif
    return prev;
}

static void *__coalesce_none(void *bp) {
    // tweak list
    if (list_p == NULL) {
        list_p = bp;
        SET(POS_SUCC(list_p), (size_t)list_p);
        SET(POS_PRED(list_p), (size_t)list_p);
    } else {
        // add to list
        SET(POS_SUCC(bp), GET_SUCC(list_p));
        SET(POS_PRED(bp), (size_t)list_p);
        SET(POS_PRED(GET_SUCC(list_p)), (size_t)bp);
        SET(POS_SUCC(list_p), (size_t)bp);
        list_p = bp;
    }
#ifdef DEBUG
    printf("---coalesce none---\n");
    _check();
    printf("-------------------\n");
#endif
    return bp;
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

static void *_first_fit_of_clear(size_t size) {
    void *bp = list_p;
    if (bp == NULL) return NULL;
    do {
        if (SIZE(bp) >= size) {
            _place(bp, size);
            return bp;
        }
        bp = (void *)GET_SUCC(bp);
    } while (bp != list_p);
    return NULL;
}

static void *_best_fit_of_clear(size_t size) {
    void *bp = list_p;
    if (bp == NULL) return NULL;
    size_t min = 0;
    void *min_p = NULL;
    do {
        if (SIZE(bp) >= size) {
            if (min_p == NULL || SIZE(bp) < min) {
                min = SIZE(bp);
                min_p = bp;
            }
        }
        bp = (void *)GET_SUCC(bp);
    } while (bp != list_p);
    if (min_p == NULL) return NULL;
    _place(min_p, size);
    return min_p;
}

static void _place(void *ptr, size_t size) {
    size_t p_size = SIZE(ptr);
    if (p_size - size >= MIN_BLOCK) {
        SET(HEAD(ptr), PACK(size, 1));
        SET(FOOT(ptr), PACK(size, 1));
        // DSIZE adjust
        size_t adjust_size = p_size - size - DSIZE;
        void *new_ptr = NEXT(ptr);
        SET(HEAD(new_ptr), PACK(adjust_size, 0));
        SET(FOOT(new_ptr), PACK(adjust_size, 0));
        if (ptr == tail_p) tail_p = new_ptr;
        // tweak list
        _fix_list(ptr, new_ptr);
        if (ptr == list_p) list_p = new_ptr;
    } else {
        SET(HEAD(ptr), PACK(p_size, 1));
        SET(FOOT(ptr), PACK(p_size, 1));
        // tweak list
        // remove
        SET(POS_SUCC(GET_PRED(ptr)), GET_SUCC(ptr));
        SET(POS_PRED(GET_SUCC(ptr)), GET_PRED(ptr));
        if (ptr == list_p) {
            if (GET_SUCC(ptr) == (size_t)ptr) list_p = NULL;
            else list_p = (void *)GET_SUCC(ptr);
        }
    }
//#ifdef DEBUG
//        printf("----place----\n");
//        _check();
//        printf("-------------\n");
//#endif
}

static void _fix_list(void *in, void *out) {
    if (GET_SUCC(in) == (size_t)in) {
        SET(POS_SUCC(out), (size_t)out);
        SET(POS_PRED(out), (size_t)out);
    } else {
        SET(POS_SUCC(out), GET_SUCC(in));
        SET(POS_PRED(out), GET_PRED(in));
        SET(POS_SUCC(GET_PRED(in)), (size_t)out);
        SET(POS_PRED(GET_SUCC(in)), (size_t)out);
    }
}

static void _check() {
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    void *bp = front_p;
    void *after_p = NEXT(tail_p);
    void *blank_p = NULL;
    while (bp != after_p) {
        if (!ALLOC(bp)) {
            if (blank_p == NULL) blank_p = bp;
            num1++;
        }
        bp = NEXT(bp);
    }
    bp = blank_p;
    do {
        if (bp != NULL) {
            bp = (void *)GET_SUCC(bp);
            num2++;
        }
    } while (bp != blank_p);

    bp = list_p;
    do {
        if (bp != NULL) {
            bp = (void *)GET_SUCC(bp);
            num3++;
        }
    } while (bp != list_p);

    printf("expect: %d, actual: %d, list_p: %d\n", num1, num2, num3);
}
