#include <stdio.h>
#include <unistd.h>
#include <limits.h>

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
    }
    else
        bp->s.ptr = p->s.ptr;

    if (p + p->s.size == bp)
    {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    }
    else
        p->s.ptr = bp;
    freep = p;
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
    char *p = malloc(100);
    printf("%p\n", (void *)p);
    free(p);
    free(p);

    char *p0 = malloc(0); // returns NULL
    printf("%p\n", (void *)p0);
    free(p0);

    char *pmax = malloc(UINT_MAX); // returns NULL due to overflow
    printf("%p\n", (void *)pmax);
    free(pmax);

    free(NULL);

    return 0;
}