#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXWORD 100
#define NOISE_WORDS_COUNT (sizeof(noise_words) / sizeof(noise_words[0]))

char *noise_words[] = {"and", "at", "in", "or", "the"};

int getword(char *word, int limit);
int binsearch(const char *word, char *tab[], int n);
struct tnode *addtree(struct tnode *p, char *w, int lnum);
struct lnumlst *addlnum(struct lnumlst *ll, int lnum);
void treeprint(struct tnode *p);
void llprint(struct lnumlst *l);
struct tnode *talloc(void);
struct lnumlst *lnumalloc(void);

struct lnumlst /* linked list of line numbers */
{
    int lnum;
    struct lnumlst *lnumnext;
};

struct tnode /* tree node of words with their line numbers */
{
    char *word;
    struct lnumlst *lnum;
    struct tnode *left;
    struct tnode *right;
};

int main(void)
{
    char word[MAXWORD];
    int currlnum = 1;
    int c;
    struct tnode *root = NULL;

    while ((c = getword(word, MAXWORD)) != EOF)
    {
        if (c == '\n') /* increment line number */
            currlnum++;
        else if (isalnum(word[0]) && binsearch(word, noise_words, NOISE_WORDS_COUNT) == -1) /* not a noise word */
            root = addtree(root, word, currlnum);
    }

    treeprint(root);
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

/* addtree: add a node with w at or below p & add line number to linked list */
struct tnode *addtree(struct tnode *p, char *w, int lnum)
{
    int cond;

    if (p == NULL) /* new word */
    {
        p = talloc(); /* make a new word */
        p->word = strdup(w);
        p->lnum = addlnum(p->lnum, lnum);
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0) /* equal to the current node */
        p->lnum = addlnum(p->lnum, lnum);      /* Add line number to linked list if not exists */
    else if (cond < 0)                         /* less than into left subtree */
        p->left = addtree(p->left, w, lnum);
    else /* greater than into right subtree */
        p->right = addtree(p->right, w, lnum);

    return p;
}

/* addlnumlst: add line number to linked list */
struct lnumlst *addlnum(struct lnumlst *llnum, int lnum)
{
    struct lnumlst *p = NULL;
    struct lnumlst *ll = llnum;

    if (ll == NULL) /* linked list is empty, create the first node */
    {
        ll = lnumalloc();
        ll->lnum = lnum;
        ll->lnumnext = NULL;
        return ll;
    }

    while (ll->lnumnext != NULL && ll->lnum != lnum) /* stop if the line number already exists or traverse till end */
        ll = ll->lnumnext;

    if (ll->lnum != lnum) /* add a new line number node at the end of the list if it doesn't already exist */
    {
        p = lnumalloc();
        p->lnum = lnum;
        p->lnumnext = NULL;
        ll->lnumnext = p;
    }

    return llnum;
}

/* treeprint: in-order print of tree */
void treeprint(struct tnode *p)
{
    if (p != NULL)
    {
        treeprint(p->left);
        printf("%s ", p->word);
        if (p->lnum != NULL) /* print the linked list of line numbers */
            llprint(p->lnum);
        printf("\n");
        treeprint(p->right);
    }
}

/* llprint: print the linked list of line numbers */
void llprint(struct lnumlst *l)
{
    while (l != NULL)
    {
        printf("%d ", l->lnum);
        l = l->lnumnext;
    }
}

/* talloc: make a tnode */
struct tnode *talloc(void)
{
    return (struct tnode *)malloc(sizeof(struct tnode));
}

/* lnumalloc: allocate memory for a new line number list node */
struct lnumlst *lnumalloc(void)
{
    return (struct lnumlst *)malloc(sizeof(struct lnumlst));
}

/* strdup: make a duplicate of s */
char *strdup(const char *s)
{
    char *p;

    p = (char *)malloc(strlen(s) + 1);
    if (p != NULL)
        strcpy(p, s);
    return p;
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

/* binsearch: find word in tab[0]...tab[n-1] - case insensitive */
int binsearch(const char *word, char *tab[], int n)
{
    int cond, mid;

    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;
        if ((cond = strcasecmp(word, tab[mid])) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}