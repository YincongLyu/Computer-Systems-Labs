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

/* Base constants and macros */
#define wsize		4		
#define dsize		8		
#define chunksize	(1<<12)	

#define Max(x, y) ((x) > (y) ? (x) : (y))


#define pack(size, alloc)	((size) | (alloc))


#define get(p)		(*(unsigned int *)(p))
#define put(p, val)	(*(unsigned int *)(p) = val)


#define get_p(p)	(*(unsigned long *)(p))
#define set_p(p, ptr)	(*(unsigned long *)(p) = (unsigned long)(ptr))


#define get_pred(p)	((char *)(*(unsigned long *)(p)))
#define get_succ(p)	((char *)(*(unsigned long *)(p + dsize)))
#define set_pred(p, ptr)	(set_p((char *)(p), ptr))
#define set_succ(p, ptr)	(set_p(((char *)(p)+(dsize)), ptr))


#define get_size(p)		(get(p) & ~0x7)
#define get_alloc(p)	(get(p) & 0x1)


#define Hdrp(bp)	((char* )(bp) - wsize)
#define Ftrp(bp)	((char* )(bp) + get_size(Hdrp(bp)) - dsize)


#define Next_blkp(bp)	((char *)(bp) + get_size(((char *)(bp) - wsize)))
#define Prev_blkp(bp)	((char *)(bp) - get_size(((char *)(bp) - dsize)))


static char * list_head;
static char * list_tail;
                                                                                             


static void* extend_heap(size_t words);
static void* coalesce(void * bp);
static void* find_fit(size_t size);
static void place(void* bp,size_t size);


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    
	if ((list_head = mem_sbrk(4*wsize+3*dsize)) == (void *)-1) {
		return -1;
	}
	put(list_head, pack(0, 0));					
	put(list_head+wsize, pack(24, 1));			
	list_head += dsize;							
	list_tail = Next_blkp(list_head);		
	set_pred(list_head, NULL);					
	set_succ(list_head, list_tail);			
	put(list_head+(2*dsize), pack(24, 1));		

	put(Hdrp(list_tail), pack(0, 1));				
	set_pred(list_tail, list_head);			
	put(list_tail+dsize, pack(0, 0));				
	
	if (extend_heap(chunksize/wsize) == NULL) {
		return -1;
	}
    
    return 0;

}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;		
    size_t extendsize;	
   	char *bp;

   	
   	if (size == 0)
   		return NULL;
   	
   
	if (size <= 2*dsize) {
		asize = 3*dsize;
	} else {
		asize = dsize * ((size + (dsize) + (dsize-1)) / dsize);
	}

   
   	if ((bp = find_fit(asize)) != NULL) {
   		place(bp, asize);
   		return bp;
   	}
   	
   
   	extendsize = Max(asize, chunksize);
   	if ((bp = extend_heap(extendsize/wsize)) == NULL)
   		return NULL;
   	place(bp, asize);
    
   	return bp; 	

}
static void*find_fit(size_t asize)
{
   void* p;

	for (p = get_succ(list_head); p != list_tail; p = get_succ(p)) {
		if (asize <= get_size(Hdrp(p)))
			return p;
	}

	return NULL;

}
                                                                                                                                                                                         

static void place(void* bp ,size_t asize)
{
    size_t size = get_size(Hdrp(bp));
	void *ptr;

	if (size - asize >= 3*dsize) {
		put(Hdrp(bp), pack(asize, 1));
		put(Ftrp(bp), pack(asize, 1));
		ptr = Next_blkp(bp);
		
		set_succ(ptr, get_succ(bp));
		set_pred(ptr, get_pred(bp));
		
		set_succ(get_pred(ptr), ptr);
		set_pred(get_succ(ptr), ptr);
		
		put(Hdrp(ptr), pack(size-asize, 0));
		put(Ftrp(ptr), pack(size-asize, 0));
		
	} else {
		put(Hdrp(bp), pack(size, 1));
		put(Ftrp(bp), pack(size, 1));

		
		set_succ(get_pred(bp), get_succ(bp));
		set_pred(get_succ(bp), get_pred(bp));
	}

}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{                                                                                                                                                                                                                                                                                             
   char *p;
	size_t size = get_size(Hdrp(ptr));

	
	for (p = get_succ(list_head); ; p = get_succ(p)) {
		if (ptr < (void *)p) {		
			put(Hdrp(ptr), pack(size, 0));
			put(Ftrp(ptr), pack(size, 0));
			set_succ(ptr, p);
			set_pred(ptr, get_pred(p));
			
			set_succ(get_pred(p), ptr);
			set_pred(p, ptr);
			break;
		}
	}

	coalesce(ptr);

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
	void *nextptr;
	void *pred;
	void *succ;
	char *p;

	size_t blockSize;
    size_t extendsize;
	size_t asize;	
	size_t sizesum;

	if (ptr == NULL) { 			
		return mm_malloc(size);
	} else if (size == 0) { 	
		mm_free(ptr);
		return NULL;
	}

	
	if (size <= 2*dsize) {
		asize = 3*dsize;
	} else {
		asize = dsize * ((size + (dsize) + (dsize-1)) / dsize);
	}

	blockSize = get_size(Hdrp(ptr));

	
	if (asize == blockSize) {						
		return ptr;
	} else if (asize < blockSize) {					
		if (blockSize-asize >= 3*dsize) {	
			
			put(Hdrp(ptr), pack(asize, 1));
			put(Ftrp(ptr), pack(asize, 1));
			
			nextptr = Next_blkp(ptr);
			put(Hdrp(nextptr), pack(blockSize-asize, 0));
			put(Ftrp(nextptr), pack(blockSize-asize, 0));
			
			for (p = get_succ(list_head); ; p = get_succ(p)) {
				if (nextptr < (void *)p) {
					pred = get_pred(p);
					succ = p;
					set_pred(nextptr, pred);
					set_succ(nextptr, succ);
					set_succ(pred, nextptr);
					set_pred(p, nextptr);
					break;
				}
			}
		} 
		
		return ptr;
	} else {										
		
		nextptr = Next_blkp(ptr);
		sizesum = get_size(Hdrp(nextptr))+blockSize;	
		if (!get_alloc(Hdrp(nextptr)) && sizesum >= asize) {	
			
			pred = get_pred(nextptr);
			succ = get_succ(nextptr);
			if (sizesum-asize >= 3*dsize) {
				put(Hdrp(ptr), pack(asize, 1));
				put(Ftrp(ptr), pack(asize, 1));
				
				nextptr = Next_blkp(ptr);
				put(Hdrp(nextptr), pack(sizesum-asize, 0));
				put(Ftrp(nextptr), pack(sizesum-asize, 0));
				
				set_pred(nextptr, pred);
				set_succ(nextptr, succ);
				set_succ(pred, nextptr);
				set_pred(succ, nextptr);
			} else {
				put(Hdrp(ptr), pack(sizesum, 1));
				put(Ftrp(ptr), pack(sizesum, 1));
				
				set_succ(pred, succ);
				set_pred(succ, pred);
			}
			return ptr;
		} else {		
			newptr = find_fit(asize);
			if (newptr == NULL) {
				extendsize = Max(asize, chunksize);
				if ((newptr = extend_heap(extendsize/wsize)) == NULL) {	
					return NULL;
				}
			}
			place(newptr, asize);
			memcpy(newptr, oldptr, blockSize-2*wsize);
			mm_free(oldptr);
			return newptr;
		}
	}
                                               
    }

static void*extend_heap(size_t words)
{
    char *bp;
	char *ptr;
	size_t size;
	
	
	size = (words % 2) ? (words+1) * wsize : words * wsize;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;
	
	
	ptr = list_tail;
	put(Hdrp(ptr), pack(size, 0));		
	put(Ftrp(ptr), pack(size, 0));	
	list_tail = Next_blkp(ptr);
	set_succ(ptr, list_tail);		

	put(Hdrp(list_tail), pack(0, 1));
	set_pred(list_tail, ptr);
	put(list_tail+dsize, pack(0, 0));
	
	return coalesce(ptr);
    
}

static void*coalesce(void*ptr)
{
    size_t prev_alloc = get_alloc(Ftrp(Prev_blkp(ptr)));
	size_t next_alloc = get_alloc(Hdrp(Next_blkp(ptr)));
	size_t size = get_size(Hdrp(ptr));
	
	void *prevptr = Prev_blkp(ptr);
	void *nextptr = Next_blkp(ptr);
	if (prev_alloc && next_alloc) {				
		return ptr;
	} else if (prev_alloc && !next_alloc) {		
		size += get_size(Hdrp(nextptr));
		set_succ(ptr, get_succ(nextptr));
		set_pred(get_succ(nextptr), ptr);
		put(Hdrp(ptr), pack(size, 0));
		put(Ftrp(nextptr), pack(size, 0));
	} else if (!prev_alloc && next_alloc) {		
		size += get_size(Ftrp(prevptr));
		set_succ(prevptr, get_succ(ptr));
		set_pred(get_succ(ptr), prevptr);
		put(Hdrp(prevptr), pack(size, 0));
		put(Ftrp(ptr), pack(size, 0));
		ptr = prevptr;
	} else if (!prev_alloc && !next_alloc) {	
		size += get_size(Hdrp(prevptr)) 
			+ get_size(Ftrp(nextptr));
		set_succ(prevptr, get_succ(nextptr));
		set_pred(get_succ(nextptr), prevptr);
		put(Hdrp(prevptr), pack(size, 0));
		put(Ftrp(nextptr), pack(size, 0));
		ptr = prevptr;
	}
    
	return ptr;

}












