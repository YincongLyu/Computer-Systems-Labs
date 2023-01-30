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
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/***********************红黑树*********************************************/
#define COLOR_RED 0
#define COLOR_BLACK 1

#define LCHILD(pt) (*((void **)pt))
#define RCHILD(pt) (*((void **)pt + 1))
#define LENGTH(pt) (*((int *)pt + 4))
#define COLOR(pt) (pt ? *((int *)pt + 5) : COLOR_BLACK)
#define ADDRLCHILD(pt) ((void **)pt)
#define ADDRRCHILD(pt) ((void **)pt + 1)
#define LRCHILD(pt) (LCHILD(pt) ? LCHILD(pt) : RCHILD(pt))
#define SETLCHILD(pt, loc) *(void **)pt = loc
#define SETRCHILD(pt, loc) *((void **)pt + 1) = loc
#define SETPLENGTH(pt, leng) *((int *)pt + 4) = leng
#define SETCOLOR(pt, color) *((int *)pt + 5) = color
/**************************************************************************/

/*********************************宏定义************************************/
#define SIZEOFHEAD 16

#define BLOCK_AVI 0  // 可以被重新分配的内存块，长度大于23
#define BLOCK_A_8 1  // 长度为8的空闲内存块
#define BLOCK_A_16 2 // 长度为16的空闲内存块
#define BLOCK_USD 3  // 已被分配的内存块

#define TYPEMASK 3
#define SIZEMASK 0xFFFFFFF8

#define ADD_ADDR(pt, sz) ((char *)pt + (sz))

#define CSIZE(pt) (*((int *)pt) & SIZEMASK)
#define FRONTCSIZE(pt) (*((int *)pt - 2) & SIZEMASK)
// length和type共享空间
#define TYPE(pt) (*((int *)pt) & TYPEMASK)
#define FRONTTYPE(pt) (*((int *)pt - 2) & TYPEMASK)

#define INITC_USD(pt, sz)                     \
    *(int *)ADD_ADDR(pt, 0) = sz + BLOCK_USD; \
    *(int *)ADD_ADDR(pt, sz - 8) = sz + BLOCK_USD

#define INITC_A_8(pt) *((int *)pt) = BLOCK_A_8

#define INITC_A_16(pt)         \
    *((int *)pt) = BLOCK_A_16; \
    *((int *)pt + 2) = BLOCK_A_16

#define INITC_AVI(pt, sz)      \
    *((void **)pt) = NULL;     \
    *((void **)pt + 1) = NULL; \
    SETPLENGTH(pt, sz);        \
    SETCOLOR(pt, COLOR_RED);   \
    *(int *)ADD_ADDR(pt, sz - 8) = sz + BLOCK_AVI

/**************************************************************************/

/*************************************函数声明******************************/
// 向树插入地址为i_p，类型为BLOCK_AVI的块
static void insert_p(void **head, void *i_p);
static int insert_p_t(void *p, void **father, const int len, void *i_p);
inline static void rotateL(void **father, void *p);
inline static void rotateR(void **father, void *p);
inline static void rotateLR(void **father, void *p);
inline static void rotateRL(void **father, void *p);
// 删除地址为p的块
static void delete_p(void **head, void *p);
static int delete_p_t(void *p, void **father, void *p_d, int leng);
inline static void *find_next(void **father, void *p, void *p_d);
// 返回树中长度不小于leng的节点
static void *find_p(void *head, int leng);
static void *find_p_t(void *p, int leng);

static void *mem_my_sbrk(int size);
/**************************************************************************/

/************************************变量声明*******************************/
static void *head__ = NULL;       // 指向树根
static void *heaptop__ = NULL;    // 堆顶
static void *heapbottom__ = NULL; // 堆底
/**************************************************************************/
/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    void *p = mem_my_sbrk(256);
    INITC_AVI(p, 256);
    insert_p(&head__, p);
    return 0;
}

void *mem_my_sbrk(int size)
{
    if (heapbottom__ == NULL)
    {
        heapbottom__ = mem_sbrk(size);
        heaptop__ = ADD_ADDR(heapbottom__, size);
        return heapbottom__;
    }
    void *ret = heaptop__;
    if (ADD_ADDR(heaptop__, size - 1) > mem_heap_hi())
    {
        int size_r = size - ((long long)mem_heap_hi() - (long long)heaptop__ + 1);
        mem_sbrk(size_r);
    }
    heaptop__ = ADD_ADDR(heaptop__, size);
    return ret;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int sizechunk = ALIGN(size) + SIZEOFHEAD;
    void *p = find_p(head__, sizechunk);

    if (p)
    {
        int newsize = LENGTH(p) - sizechunk;
        void *newp = (void *)ADD_ADDR(p, sizechunk);
        delete_p(&head__, p);
        INITC_USD(p, sizechunk);

        if(newsize==8)
        {
            INITC_A_8(newp);
        }
        else if(newsize==16)
        {
            INITC_A_16(newp);
        }
        else if(newsize!=0)
        {
            INITC_AVI(newp, newsize);
            insert_p(&head__, newp);
        }
        return (void *)((char *)p + ALIGNMENT);
    }
    else
    {
        p = mem_my_sbrk(sizechunk);
        INITC_USD(p, sizechunk);
        return (void *)((char *)p + ALIGNMENT);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    // 采用立即合并的方式
    // 1.前后块均为空闲块，从树中删除，然后把3个块合并为1个新的空，再插入树
    // 2.前块为已分配或是堆底，后块空，先删，然后把后2块合并为新的空，再插入树
    // 3.前块空...
    void *pc = (void *)ADD_ADDR(ptr, -8);
    int sizec_avi = CSIZE(pc);
    void *nextp = (void *)ADD_ADDR(pc, sizec_avi);
    int size_front;
    if (pc < heapbottom__ || pc > heaptop__)
    {
        return;
    }
    if (pc != heapbottom__)
    {
        if(FRONTTYPE(pc) == BLOCK_AVI)
        {
            size_front = FRONTCSIZE(pc);
            sizec_avi += size_front;
            pc = (void *)ADD_ADDR(pc, -size_front);
            delete_p(&head__, pc);
        }
        else if (FRONTTYPE(pc) == BLOCK_A_16)
        {
            sizec_avi += 16;
            pc = (void *)ADD_ADDR(pc, -16);
        }
        else if(FRONTTYPE(pc) == BLOCK_A_8)
        {
            sizec_avi += 8;
            pc = (void *)ADD_ADDR(pc, -8);
        }
    }
    if (nextp < heaptop__)
    {

        if(TYPE(nextp)==BLOCK_AVI)
        {
            sizec_avi += LENGTH(nextp);
            delete_p(&head__, nextp);
        }
        else if(TYPE(nextp)==BLOCK_A_16)
        {
            sizec_avi += 16;
        }
        else if(TYPE(nextp)==BLOCK_A_8)
        {
            sizec_avi += 8;
        }
    }
    else
    {
        heaptop__ = ADD_ADDR(heaptop__, -sizec_avi);
        return;
    }
    INITC_AVI(pc, sizec_avi);
    insert_p(&head__, pc);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        return mm_malloc(size);
    }
    void *pc = (void *)ADD_ADDR(ptr, -8);
    int sizechunck = ALIGN(size) + SIZEOFHEAD;
    int old_size = CSIZE(pc);
    // 1.如果重新分配的空间大小大于原来的时
    //  （1）该块位于堆顶，直接在堆顶分配对应空间，新分配的空间大于指定的空间。
    //  （2）该块不在堆顶，把该块free掉，重新分配空间，也要留下多余空间防止被不停realloc
    if (sizechunck <= old_size)
    {
        if (sizechunck << 1 >= old_size)
        {
            return ptr;
        }
        else
        {
            void *mid_pc = ADD_ADDR(pc, sizechunck);
            int mid_size = old_size - sizechunck;

            INITC_USD(pc, sizechunck);
            INITC_USD(mid_pc, mid_size);
            mm_free(ADD_ADDR(mid_pc, 8));

            return ptr;
        }
    }
    // 2.如果重分配的空间大小小于原来时
    // （1）分配值大于原来的一半，直接返回ptr
    // （2）分配值小于原来的一半，把原来空间分割为两份。空的部分重新插入树中
    else
    {
        if (old_size > 16000)
        {
            if (old_size + 1024 > sizechunck)
                sizechunck = ALIGN(old_size + 1024) + 16;
        }
        else
        {
            if (old_size + 256 > sizechunck)
                sizechunck = ALIGN(old_size + 256) + 16;
        }

        if (ADD_ADDR(pc, old_size) >= heaptop__)
        {
            mem_my_sbrk(sizechunck - old_size);
            INITC_USD(pc, sizechunck);
            return ptr;
        }
        else
        {
            void *new_ptr = mm_malloc(size);
            int i;
            for (i = 0; i < (old_size - SIZEOFHEAD) / 8; i += 4)
            {
                *((long long *)new_ptr + i) = *((long long *)ptr + i);
                *((long long *)new_ptr + i + 1) = *((long long *)ptr + i + 1);
                *((long long *)new_ptr + i + 2) = *((long long *)ptr + i + 2);
                *((long long *)new_ptr + i + 3) = *((long long *)ptr + i + 3);
            }
            for (i -= 4; i < (old_size - SIZEOFHEAD) / 8; i++)
            {
                *((long long *)new_ptr + i) = *((long long *)ptr + i);
            }
            mm_free(ptr);
            return new_ptr;
        }
    }
}

// 实现红黑树
static void insert_p(void **head, void *i_p)
{
    if (*head == NULL)
        *head = i_p;
    else
        insert_p_t(*head, head, LENGTH(i_p), i_p);
    SETCOLOR(*head, COLOR_BLACK);
}

static int insert_p_t(void *p, void **father, const int len, void *i_p)
{

    int needreset = 0;

    if (len <= LENGTH(p))
    {
        if (LCHILD(p) != NULL)
            needreset = insert_p_t(LCHILD(p), ADDRLCHILD(p), len, i_p);
        else
            SETLCHILD(p, i_p);
        void *lc = LCHILD(p);
        void *rc = RCHILD(p);

        if (needreset)
        {
            if (COLOR(rc) == COLOR_RED)
            {
                SETCOLOR(p, COLOR_RED);
                SETCOLOR(lc, COLOR_BLACK);
                SETCOLOR(rc, COLOR_BLACK);
            }
            else
            {
                if (COLOR(LCHILD(lc)) == COLOR_RED)
                    rotateR(father, p);
                else
                    rotateLR(father, p);
            }
            return 0;
        }
        else
        {
            if (COLOR(p) == COLOR_RED)
                if (COLOR(lc) == COLOR_RED || COLOR(rc) == COLOR_RED)
                    return 1;
            return 0;
        }
    }
    else
    {
        if (RCHILD(p) != NULL)
            needreset = insert_p_t(RCHILD(p), ADDRRCHILD(p), len, i_p);
        else
            SETRCHILD(p, i_p);
        void *rc = RCHILD(p);
        void *lc = LCHILD(p);

        if (needreset)
        {
            if (COLOR(lc) == COLOR_RED)
            {
                SETCOLOR(p, COLOR_RED);
                SETCOLOR(lc, COLOR_BLACK);
                SETCOLOR(rc, COLOR_BLACK);
            }
            else
            {
                if (COLOR(RCHILD(rc)) == COLOR_RED)
                    rotateL(father, p);
                else
                    rotateRL(father, p);
            }
            return 0;
        }
        else
        {
            if (COLOR(p) == COLOR_RED)
                if (COLOR(lc) == COLOR_RED || COLOR(rc) == COLOR_RED)
                    return 1;
            return 0;
        }
    }
    return 0;
}

inline static void rotateR(void **father, void *p)
{
    void *pB = LCHILD(p);
    void *pBrc = RCHILD(pB);
    int t_color = COLOR(pB);

    *father = pB;
    SETRCHILD(pB, p);
    SETLCHILD(p, pBrc);

    SETCOLOR(pB, COLOR(p));
    SETCOLOR(p, t_color);
}

inline static void rotateL(void **father, void *p)
{
    void *pB = RCHILD(p);
    void *pBlc = LCHILD(pB);
    int t_color = COLOR(pB);

    *father = pB;
    SETLCHILD(pB, p);
    SETRCHILD(p, pBlc);

    SETCOLOR(pB, COLOR(p));
    SETCOLOR(p, t_color);
}

inline static void rotateLR(void **father, void *p)
{
    void *pB = LCHILD(p);
    void *pC = RCHILD(pB);
    int t_color = COLOR(pC);

    *father = pC;
    SETRCHILD(pB, LCHILD(pC));
    SETLCHILD(p, RCHILD(pC));
    SETRCHILD(pC, p);
    SETLCHILD(pC, pB);

    SETCOLOR(pC, COLOR(p));
    SETCOLOR(p, t_color);
}

inline static void rotateRL(void **father, void *p)
{
    void *pB = RCHILD(p);
    void *pC = LCHILD(pB);
    int t_color = COLOR(pC);

    *father = pC;
    SETLCHILD(pB, RCHILD(pC));
    SETRCHILD(p, LCHILD(pC));
    SETLCHILD(pC, p);
    SETRCHILD(pC, pB);

    SETCOLOR(pC, COLOR(p));
    SETCOLOR(p, t_color);
}

static void delete_p(void **head, void *p)
{
    if (*head == p)
        if (LCHILD(p) == NULL || RCHILD(p) == NULL)
        {
            *head = LRCHILD(p);
            if (*head)
                SETCOLOR(*head, COLOR_BLACK);
            return;
        }
    delete_p_t(*head, head, p, LENGTH(p));
}

static int delete_p_t(void *p, void **father, void *p_d, int leng)
{

    if (p == p_d)
    {

        if (LCHILD(p) && RCHILD(p))
        {

            void *p_d_father;
            p = find_next(&p_d_father, RCHILD(p_d), p_d);
            void *p_d_child = LRCHILD(p);

            *father = p;

            if (p_d_father == p_d)
                SETRCHILD(p, p_d);
            else
            {
                SETRCHILD(p, RCHILD(p_d));
                SETLCHILD(p_d_father, p_d);
            }
            SETLCHILD(p, LCHILD(p_d));

            SETLCHILD(p_d, p_d_child);
            SETRCHILD(p_d, NULL);

            int t_color = COLOR(p);
            SETCOLOR(p, COLOR(p_d));
            SETCOLOR(p_d, t_color);

            leng = LENGTH(p);
            SETPLENGTH(p_d, leng);
        }
        else
            return -1;
    }

    int p_leng = LENGTH(p);

    if (leng <= p_leng)
    {
        int opt = 0;

        if (LCHILD(p))
            opt = delete_p_t(LCHILD(p), ADDRLCHILD(p), p_d, leng);

        void *lc = LCHILD(p);
        void *rc = RCHILD(p);

        switch (opt)
        {
        case -1:
            SETLCHILD(p, LRCHILD(lc));
            if (COLOR(lc) == COLOR_RED)
                return 2;
        case 1:
            if (COLOR(p) == COLOR_BLACK && COLOR(RCHILD(rc)) == COLOR_BLACK && COLOR(LCHILD(rc)) == COLOR_BLACK)
            {
                if (COLOR(rc) == COLOR_RED)
                {
                    void **rcaddr = ADDRLCHILD(rc);
                    rotateL(father, p);
                    father = rcaddr;
                    rc = RCHILD(p);
                }
                else
                {
                    SETCOLOR(rc, COLOR_RED);
                    return 1;
                }
            }
            if (COLOR(RCHILD(rc)) == COLOR_RED)
            {
                rotateL(father, p);
                SETCOLOR(RCHILD(rc), COLOR_BLACK);
                return 2;
            }
            if (COLOR(LCHILD(rc)) == COLOR_RED)
            {
                rotateRL(father, p);
                SETCOLOR(p, COLOR_BLACK);
                return 2;
            }
            SETCOLOR(p, COLOR_BLACK);
            SETCOLOR(rc, COLOR_RED);
        case 2:
            return 2;
        case 0:
            break;
        }
    }
    if (leng >= p_leng)
    {
        int opt = 0;

        if (RCHILD(p))
            opt = delete_p_t(RCHILD(p), ADDRRCHILD(p), p_d, leng);

        void *lc = LCHILD(p);
        void *rc = RCHILD(p);

        switch (opt)
        {
        case -1:
            SETRCHILD(p, LRCHILD(rc));
            if (COLOR(rc) == COLOR_RED)
                return 2;
        case 1:
            if (COLOR(p) == COLOR_BLACK && COLOR(RCHILD(lc)) == COLOR_BLACK && COLOR(LCHILD(lc)) == COLOR_BLACK)
            {
                if (COLOR(lc) == COLOR_RED)
                {
                    void **lcaddr = ADDRRCHILD(lc);
                    rotateR(father, p);
                    father = lcaddr;
                    lc = LCHILD(p);
                }
                else
                {
                    SETCOLOR(lc, COLOR_RED);
                    return 1;
                }
            }
            if (COLOR(LCHILD(lc)) == COLOR_RED)
            {
                rotateR(father, p);
                SETCOLOR(LCHILD(lc), COLOR_BLACK);
                return 2;
            }
            if (COLOR(RCHILD(lc)) == COLOR_RED)
            {
                rotateLR(father, p);
                SETCOLOR(p, COLOR_BLACK);
                return 2;
            }
            SETCOLOR(p, COLOR_BLACK);
            SETCOLOR(lc, COLOR_RED);
        case 2:
            return 2;
        case 0:
            break;
        }
    }
    return 0;
}

inline static void *find_next(void **father, void *p, void *pre)
{
    void *lc;
    while ((lc = LCHILD(p)) != NULL)
        pre = p, p = lc;
    *father = pre;
    return p;
}

static void *find_p(void *head, int leng)
{
    if (head)
        return find_p_t(head, leng);
    else
        return NULL;
}

static void *find_p_t(void *p, int leng)
{

    if (leng == LENGTH(p))
        return p;
    if (leng < LENGTH(p))
    {
        void *q;
        if (LCHILD(p))
            q = find_p_t(LCHILD(p), leng);
        else
            return p;
        if (q)
            return q;
        else
            return p;
    }
    else
    {
        if (RCHILD(p))
            return find_p_t(RCHILD(p), leng);
        else
            return NULL;
    }
}