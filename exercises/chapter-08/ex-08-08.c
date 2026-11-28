#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

typedef long Align; /* for alignment to long boundary */

union header
{
    struct
    {
        union header *ptr; /* next bock if on free list */
        unsigned size;
    } s;
    Align x; /*force alignment of blocks */
};

typedef union header Header;

static Header base;
static Header *freep = NULL; /* start of free list */
static unsigned maxunits = 0;

void *malloc(unsigned int);
void free(void *);
static Header *morecore(unsigned);
void bfree(void *, unsigned int);

/* malloc: general purpose memory allocator */
void *malloc(unsigned nbytes)
{
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    if (nbytes == 0 || nbytes > (UINT_MAX - (unsigned int)sizeof(Header)))
        return NULL;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    if ((prevp = freep) == NULL) /* no free list yet */
    {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
    {
        if (p->s.size >= nunits) /* big enough*/
        {
            if (p->s.size == nunits) /*exactly*/
                prevp->s.ptr = p->s.ptr;
            else /* allocate tail end */
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep) /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL; /* none left */
    }
}

#define NALLOC 1024 /* minimum number of units to request */

/* morecore: ask system for more memory */
static Header *morecore(unsigned nu)
{
    char *cp;
    Header *up;
    unsigned size;

    if (nu > INT_MAX / (unsigned int)sizeof(Header)) /* overflow */
        return NULL;

    if (nu < NALLOC)
        nu = NALLOC;

    size = nu * (unsigned int)sizeof(Header);
    cp = sbrk(size);
    if (cp == (char *)-1) /* no space at all */
        return NULL;

    if (maxunits < nu)
        maxunits = nu;

    up = (Header *)cp;
    up->s.size = nu;
    free((void *)(up + 1));
    return freep;
}

/* free: put block ap in free list */
void free(void *ap)
{
    Header *bp, *p;

    if (freep == NULL || ap == NULL || ((Header *)ap - 1)->s.size == 0 || ((Header *)ap - 1)->s.size > maxunits)
        return;

    bp = (Header *)ap - 1; /* point to header block */

    for (p = freep; !(bp > p && bp < p->s.ptr) && bp != p; p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break; /* freed block at start or end of arena */

    if ((bp >= p && bp < p + p->s.size) || (bp < p->s.ptr && (bp + bp->s.size) > p->s.ptr)) /* overlap */
        return;

    if (bp + bp->s.size == p->s.ptr) /* join to upper bound */
    {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
        if (maxunits < bp->s.size)
            maxunits = bp->s.size;
    }
    else
        bp->s.ptr = p->s.ptr;

    if (p + p->s.size == bp) /* join to lower bound */
    {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
        if (maxunits < p->s.size)
            maxunits = p->s.size;
    }
    else
        p->s.ptr = bp;

    freep = p;
}

/* bfree: free arbitary block p of n characters into freelist maintained by malloc and free */
void bfree(void *p, unsigned int n)
{
    Header *bp;
    unsigned nunits;
    unsigned int rem_offset, align_offset = 0, min_size;

    if (p == NULL)
        return;

    rem_offset = (unsigned int)((uintptr_t)p % (unsigned int)sizeof(Header));

    if (rem_offset != 0)
        align_offset = (unsigned int)sizeof(Header) - rem_offset;

    min_size = align_offset + (unsigned int)sizeof(Header) +
               (unsigned int)sizeof(Header);

    if (n < min_size)
        return;

    nunits = (n - align_offset) / (unsigned)sizeof(Header);

    bp = (Header *)((char *)p + align_offset); /* set header */
    bp->s.size = nunits;
    bp = bp + 1;

    if (maxunits < nunits)
        maxunits = nunits;

    if (freep == NULL) /* no free list yet */
    {
        base.s.ptr = freep = &base;
        base.s.size = 0;
    }

    free(bp);
}

void *calloc(unsigned int count, unsigned int size)
{
    unsigned int total;

    if (size != 0 && count > (UINT_MAX) / size) /* overflow */
        return NULL;

    total = count * size;
    void *p = malloc(total);

    if (p == NULL)
        return NULL;

    for (char *i = (char *)p; i < (char *)p + total; i++)
        *i = 0;
    return p;
}

int main()
{

    char s[200];
    char w[500];
    Header *h;

    bfree(s, 200);
    bfree(w, 500);

    printf("source pointers: s: %p, w: %p\n", s, w);
    printf("[base block] pointer: %p | size: %u\n", &base, (&base)->s.size * (unsigned)sizeof(Header));
    // print merged block: s + w
    h = (&base)->s.ptr;
    printf("[block # 1] pointer: %p | size: %u\n", (char *)h, h->s.size * (unsigned)sizeof(Header));

    return 0;
}