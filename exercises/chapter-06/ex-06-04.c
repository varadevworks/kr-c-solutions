#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

struct wnode
{
    char *word; /* key */
    int count;
    struct wnode *left;
    struct wnode *right;
};

struct cwnode /* word linked list */
{
    char *word;
    struct cwnode *next;
};

struct cnode
{
    int count; /* key */
    struct cwnode *wlist;
    struct cnode *left;
    struct cnode *right;
};

/* function declarations */
int getword(char *, int lim);
int getch(void);
void ungetch(int c);

struct wnode *talloc(void);
struct cnode *ialloc(void);
struct cwnode *cwalloc(void);

struct wnode *addwtree(struct wnode *p, char *w);
struct cnode *addctree(struct cnode *p, int c, char *w);
struct cwnode *addwlist(struct cwnode *cw, char *w);
struct cnode *buildctree(struct cnode *r, struct wnode *q);

void wtreeprint(struct wnode *p);
void ctreeprint(struct cnode *p);
void wlistprint(struct cwnode *p, int c);

int main(void)
{
    char word[MAXWORD];
    struct wnode *wroot = NULL;
    struct cnode *croot = NULL;
    int c;

    /* build word tree*/
    while ((c = getword(word, MAXWORD)) != EOF)
        if (isalnum(c))
            wroot = addwtree(wroot, word);

    wtreeprint(wroot);

    /* build occurance tree */
    croot = buildctree(croot, wroot);
    ctreeprint(croot);

    return 0;
}

/* getword: get next word or character from input */
int getword(char *word, int limit)
{
    int getch(void);
    void ungetch(int c);

    char *w = word;
    int c;

    while (isspace(c = getch()) && c != '\n') /* skip whitespace except newline */
        ;

    if (c == EOF || c == '\n' || !isalnum(c)) /* not a word character */
    {
        *w = '\0';
        return c;
    }
    else /* a word character */
        *w++ = c;

    for (; --limit > 0; w++) /* read the rest of the word */
        if (!isalnum(*w = getch()))
        {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0]; /* return the first character of the word */
}

/* addwtree: add a node with w, at or below p */
struct wnode *addwtree(struct wnode *p, char *w)
{
    int cond;

    if (p == NULL) /* a new word has arrived */
    {
        p = talloc(); /* make a new node */
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;    /* repeated word */
    else if (cond < 0) /* less than into left subtree */
        p->left = addwtree(p->left, w);
    else /* greater than into right subtree */
        p->right = addwtree(p->right, w);
    return p;
}

/* wtreeprint: in-order print of tree p */
void wtreeprint(struct wnode *p)
{
    if (p != NULL)
    {
        wtreeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        wtreeprint(p->right);
    }
}

/* addctree: add a node with c, at or below p */
struct cnode *addctree(struct cnode *p, int c, char *w)
{

    if (p == NULL) /* a new occurance has arrived */
    {
        p = ialloc(); /* make a new node */
        p->count = c;
        p->wlist = addwlist(p->wlist, w);
        p->left = p->right = NULL;
    }
    else if (c == p->count)
        p->wlist = addwlist(p->wlist, w);
    else if (c < p->count) /* less than into left subtree */
        p->left = addctree(p->left, c, w);
    else /* greater than into right subtree */
        p->right = addctree(p->right, c, w);
    return p;
}

/* ctreeprint: in-order print of tree p */
void ctreeprint(struct cnode *p)
{

    if (p != NULL)
    {
        ctreeprint(p->left);
        wlistprint(p->wlist, p->count);
        ctreeprint(p->right);
    }
}

/* buildctree: build occurance + word tree */
struct cnode *buildctree(struct cnode *r, struct wnode *p)
{

    if (p != NULL)
    {
        r = buildctree(r, p->left);
        r = addctree(r, p->count, p->word);
        r = buildctree(r, p->right);
    }

    return r;
}

/* wlistprint: print word linked list */
void wlistprint(struct cwnode *p, int c)
{
    struct cwnode *r = p;

    if (p != NULL)
    {
        printf("%4d ", c);
        while (r != NULL)
        {
            printf("%s ", r->word);
            r = r->next;
        }
        printf("\n");
    }
}

/*addwlist: add word to linked list */
struct cwnode *addwlist(struct cwnode *cw, char *w)
{
    struct cwnode *p = cw;
    struct cwnode *n = NULL;

    if (p == NULL) /* add new node */
    {
        p = cwalloc();
        p->word = w;
        p->next = NULL;
        return p;
    }

    while (p->next != NULL && strcmp(p->word, w) != 0)
        p = p->next;

    if (strcmp(p->word, w) != 0)
    {
        n = cwalloc();
        n->word = w;
        n->next = NULL;
        p->next = n;
    }

    return cw;
}

/* talloc: make a tnode */
struct wnode *talloc(void)
{
    return (struct wnode *)malloc(sizeof(struct wnode));
}

/* ialloc: make a cnode */
struct cnode *ialloc(void)
{
    return (struct cnode *)malloc(sizeof(struct cnode));
}

/* cwalloc: make a cwnode */
struct cwnode *cwalloc(void)
{
    return (struct cwnode *)malloc(sizeof(struct cwnode));
}

#define BUFSIZE 100

int buf[BUFSIZE]; /* buffer for ungetch*/
int bufp = 0;     /* next free position in buffer */

/* getch/ungetch: get and push back characters */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}