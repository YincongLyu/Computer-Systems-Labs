#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"


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

#define WSIZE 4
#define DSIZE 8
#define HEAPSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define BLK_INIT(size, alloc) ((size) | (alloc)) //初始化块的header和footer

#define GET_WORD(p) (*(unsigned int *)(p))
#define PUT_WORD(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(header) (GET_WORD(header) & ~0x7)
#define GET_ALLOC(header) (GET_WORD(header) & 0x1)
#define GET_HEADER(p) ((char *)(p) - WSIZE)
#define GET_FOOTER(p) ((char *)(p) + GET_SIZE(GET_HEADER(p)) - DSIZE)

#define GET_PREV_FOOTER(p) ((char *)(p) - DSIZE)
#define NEXT_BLKP(p) ((char *)(p) + GET_SIZE(((char *)(p) - WSIZE)))
#define PREV_BLKP(p) ((char *)(p) - GET_SIZE(((char *)(p) - DSIZE)))


static char *blk_heap;
static char *next_match;
//扩展堆
static void *extend_heap(size_t words);
//块的合并
static void *blk_merge(void *blk);
//块的分割
static void *blk_split(void *blk, size_t size);
//块匹配算法
static void *blk_find(size_t size);


int mm_init(void)
{
    //首先创建一个序言块以及一个结尾块，序言块需要2个W，结尾块需要一个W，由于双字对齐，申请四个
    if((blk_heap = mem_sbrk(4 * WSIZE)) == (void *)(-1)){
        printf("init heap failed\n");
        return -1;
    }
    PUT_WORD(blk_heap, 0);//对齐字节
    //序言块占2B
    PUT_WORD(blk_heap + WSIZE, BLK_INIT(DSIZE, 1));
    PUT_WORD(blk_heap + (2 * WSIZE), BLK_INIT(DSIZE, 1));
    //结尾块，结尾块header长度要求为0，分配字段为1
    PUT_WORD(blk_heap + (3 * WSIZE), BLK_INIT(0, 1));

    blk_heap += (2 * WSIZE);//堆指向序言块的data部分，data部分为空，因此指向footer
    next_match = blk_heap;
    //创建堆的其余空间
    if(extend_heap(HEAPSIZE / WSIZE) == NULL){
        printf("create heap failed\n");
        return -1;
    }
    return 0;
}

void *mm_malloc(size_t size)
{
    size_t real_size;
    size_t extend_size;
    char *blk;

    if(size == 0){
        return NULL;
    }

    if(size <= DSIZE){
        real_size = 2 * DSIZE;
    }else{
        real_size = DSIZE + ((size + (DSIZE - 1)) / DSIZE) * DSIZE;
        //一个DSIZE用于header和footer，size + ALIGN部分计算需要的另外的DSIZE
    }
    if((blk = blk_find(real_size)) == NULL){
        //未找到合适的块，扩展堆
        extend_size = MAX(real_size, HEAPSIZE);
        if((blk = extend_heap(extend_size / WSIZE)) == NULL){
            printf("extend heap failed\n");
            return NULL;
        }
    }
    //切出需要的大小
    blk_split(blk, real_size);
    return blk;

}

void mm_free(void *ptr)
{
    size_t blk_size = GET_SIZE(GET_HEADER(ptr));
    PUT_WORD(GET_HEADER(ptr), BLK_INIT(blk_size, 0));
    PUT_WORD(GET_FOOTER(ptr), BLK_INIT(blk_size, 0));

    blk_merge(ptr);
}

void *mm_realloc(void *ptr, size_t size)
{
    void *newptr;
    size_t copySize;

    if(ptr == NULL){
        return mm_malloc(size);
    }
    if(size == 0){
        mm_free(ptr);
    }
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;

    copySize = GET_SIZE(GET_HEADER(newptr));
    size = GET_SIZE(GET_HEADER(ptr));
    if (size < copySize)
      copySize = size;
    //不拷贝footer，header和footer在malloc的时候已经设置完毕
    memcpy(newptr, ptr, copySize - WSIZE);
    mm_free(ptr);
    return newptr;
}

static void *extend_heap(size_t words){
    
    char *new_blk;
    size_t real_size;
    //字节对齐
    real_size = (words % 2) ? (words + 1) * WSIZE : (words * WSIZE);
    if((new_blk = mem_sbrk(real_size)) == -1){
        printf("extend heap failed\n");
        return NULL;
    }
    //获取到的new_blk指针不包含header，由于每次在结尾块之后扩充
    //因此拿扩充前的结尾块当作扩展部分的header，然后创建一个新的footer
    PUT_WORD(GET_HEADER(new_blk), BLK_INIT(real_size, 0));
    PUT_WORD(GET_FOOTER(new_blk), BLK_INIT(real_size, 0));
    //创建新的结尾块
    PUT_WORD(GET_HEADER(NEXT_BLKP(new_blk)), BLK_INIT(0, 1));
    //如果前一个块是空闲的，需要进行合并
    return blk_merge(new_blk);
}

static void *blk_merge(void *blk){
    size_t prev_alloc = GET_ALLOC(GET_FOOTER(PREV_BLKP(blk)));
    size_t next_alloc = GET_ALLOC(GET_HEADER(NEXT_BLKP(blk)));
    size_t size = GET_SIZE(GET_HEADER(blk));

    if(prev_alloc && next_alloc){
        return blk;
    }
    if(!prev_alloc && next_alloc){
        //合并前面的
        size += GET_SIZE(GET_HEADER(PREV_BLKP(blk)));
        PUT_WORD(GET_FOOTER(blk), BLK_INIT(size, 0));//更新当前块的footer
        PUT_WORD(GET_HEADER(PREV_BLKP(blk)), BLK_INIT(size, 0));
        blk = PREV_BLKP(blk);
    }else if(prev_alloc && !next_alloc){
        //合并后面的
        size += GET_SIZE(GET_HEADER(NEXT_BLKP(blk)));
        PUT_WORD(GET_HEADER(blk), BLK_INIT(size, 0));
        PUT_WORD(GET_FOOTER(blk), BLK_INIT(size, 0));
    }else{
        size += GET_SIZE(GET_HEADER(PREV_BLKP(blk)));
        size += GET_SIZE(GET_FOOTER(NEXT_BLKP(blk)));
        PUT_WORD(GET_HEADER(PREV_BLKP(blk)), BLK_INIT(size, 0));
        PUT_WORD(GET_FOOTER(NEXT_BLKP(blk)), BLK_INIT(size, 0));
        blk = PREV_BLKP(blk);
    }
    //发生合并时，需要更新下一次匹配算法使用的指针，防止其指向的块被合并
    if((next_match > (char *)blk) && (next_match < NEXT_BLKP(blk))){
        //此时next_match指向合并后的块的内部
        next_match = blk;
    }
    return blk;
}

static void *blk_find(size_t size){

    //char *blk = blk_heap;
    char *blk = next_match;
    size_t alloc;
    size_t blk_size;

    while(GET_SIZE(GET_HEADER(NEXT_BLKP(blk))) > 0){
        blk = NEXT_BLKP(blk);
        alloc = GET_ALLOC(GET_HEADER(blk));
        if(alloc == 1){
            continue;
        }
        blk_size = GET_SIZE(GET_HEADER(blk));
        if(blk_size < size){
            continue;
        }
        next_match = blk;
        return blk;
    }
    blk = blk_heap;
    while(blk != next_match){
        blk = NEXT_BLKP(blk);
        alloc = GET_ALLOC(GET_HEADER(blk));
        if(alloc == 1){
            continue;
        }
        blk_size = GET_SIZE(GET_HEADER(blk));
        if(blk_size < size){
            continue;
        }
        next_match = blk;
        return blk;
    }
    return NULL;
}

static void *blk_split(void *blk, size_t size){
    size_t blk_size = GET_SIZE(GET_HEADER(blk));
    size_t need_size = size;
    size_t leave_size = blk_size - need_size;
    if(leave_size >= 2 * DSIZE){
        //进行块的切分
        PUT_WORD(GET_HEADER(blk), BLK_INIT(need_size, 1));
        PUT_WORD(GET_FOOTER(blk), BLK_INIT(need_size, 1));
        PUT_WORD(GET_HEADER(NEXT_BLKP(blk)), BLK_INIT(leave_size, 0));
        PUT_WORD(GET_FOOTER(NEXT_BLKP(blk)), BLK_INIT(leave_size, 0));
    }else{
        PUT_WORD(GET_HEADER(blk), BLK_INIT(blk_size, 1));
        PUT_WORD(GET_FOOTER(blk), BLK_INIT(blk_size, 1));
    }
    return blk;
}

