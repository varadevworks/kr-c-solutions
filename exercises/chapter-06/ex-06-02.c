#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct vnode /* variable node */
{
    char *word;          /* points to variable word*/
    struct vnode *left;  /* left variable node*/
    struct vnode *right; /* right variable node*/
};

struct tnode /* group node */
{
    char *word;           /* points to the group word */
    struct tnode *left;   /* left group child */
    struct tnode *right;  /* right group child */
    struct vnode *gvnode; /* variable node*/
};

#define MAXWORD 100
struct tnode *addtree(struct tnode *p, char *w);
struct vnode *addvtree(struct vnode *p, char *w);
void treeprint(struct tnode *p);
void vtreeprint(struct vnode *p);
int getword(char *word, int lim);
struct tnode *talloc(void);
struct vnode *dalloc(void);
char *strdup(const char *s);
int check_keyword(char *s);

int glen = 6; /* default group length */

char *keywords[] = {"auto", "break", "case", "char", "const", "continue",
                    "default", "do", "double", "else", "enum", "extern", "float", "for", "goto",
                    "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
                    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", NULL};

int main(int argc, char *argv[])
{
    if (argc == 2)
        glen = atoi(argv[1]); /* set group length from command line argument*/
    else if (argc > 2 || argc != 1)
    {
        fprintf(stderr, "Usage: %s [glen]\n", argv[0]);
        return 1;
    }

    if (glen <= 0 || glen > MAXWORD)
    {
        fprintf(stderr, "Invalid group length: %d\n", glen);
        return 1;
    }

    struct tnode *root;
    char word[MAXWORD];

    root = NULL;

    while (getword(word, MAXWORD) != EOF)
        if ((isalpha(word[0]) || word[0] == '_') && !check_keyword(word))
            root = addtree(root, word);
    treeprint(root);
    return 0;
}

/* addtree: add a node with w at or below p */
struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;
    char gw[MAXWORD + 1];

    strncpy(gw, w, glen); /* copy first `glen` characters of word */
    gw[glen] = '\0';      /* ensure null-termination */

    if (p == NULL) /* new word */
    {
        p = talloc(); /* make a new word */
        p->word = strdup(gw);
        p->gvnode = addvtree(p->gvnode, w);
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(gw, p->word)) == 0) /* equal to the current node */
        addvtree(p->gvnode, w);                 /* do nothing */
    else if (cond < 0)                          /* less than into left subtree */
        p->left = addtree(p->left, w);
    else /* greater than into right subtree */
        p->right = addtree(p->right, w);

    return p;
}

/* addvtree: add a node with w at or below p */
struct vnode *addvtree(struct vnode *p, char *w)
{
    int cond;

    if (p == NULL) /* new word */
    {
        p = dalloc(); /* make a new word */
        p->word = strdup(w);
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0) /* equal to the current node */
        ;                                      /* do nothing */
    else if (cond < 0)                         /* less than into left subtree */
        p->left = addvtree(p->left, w);
    else /* greater than into right subtree */
        p->right = addvtree(p->right, w);

    return p;
}

/* treeprint: in-order print of tree */
void treeprint(struct tnode *p)
{
    if (p != NULL)
    {
        treeprint(p->left);
        printf("%s:\n", p->word);
        if (p->gvnode != NULL)
            vtreeprint(p->gvnode);
        treeprint(p->right);
    }
}

/* vtreeprint: in-order print of vnode tree */
void vtreeprint(struct vnode *p)
{
    if (p != NULL)
    {
        vtreeprint(p->left);
        printf(" %s\n", p->word);
        vtreeprint(p->right);
    }
}

/* talloc: make a tnode */
struct tnode *talloc(void)
{
    return (struct tnode *)malloc(sizeof(struct tnode));
}

/* dalloc: make a vnode */
struct vnode *dalloc(void)
{
    return (struct vnode *)malloc(sizeof(struct vnode));
}

/* strdup: make a duplicate of s */
char *strdup(const char *s) /* make a duplicate of s */
{
    char *p;

    p = (char *)malloc(strlen(s) + 1);
    if (p != NULL)
        strcpy(p, s);
    return p;
}

int check_keyword(char *word)
{
    for (int i = 0; keywords[i] != NULL; i++)
        if (strcasecmp(word, keywords[i]) == 0)
            return 1;
    return 0;
}

int getch(void);
void ungetch(int);

/* getword: get next word from input or character from input */
int getword(char *word, int lim)
{
    int skip_string_constants(void);
    int skip_comments(void);
    int skip_preprocessor_control_lines(void);
    int skip_character_constants(void);

    int c, skip = 0;
    char *w = word;

    while (isspace(c = getch()))
        ;

    switch (c)
    {
    case '"':
        skip = skip_string_constants();
        break;
    case '/':
        skip = skip_comments();
        break;
    case '#':
        skip = skip_preprocessor_control_lines();
        break;
    case '\'':
        skip = skip_character_constants();
        break;
    }

    if (c != EOF)
        *w++ = (char)c;

    if (skip || (!isalpha(c) && c != '_'))
    {
        *w = '\0';
        return c;
    }

    for (; --lim > 0; w++)
        if (!isalnum(*w = (char)getch()) && *w != '_')
        {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

/* skip_string_constants: skip over string constants and return 1 if skipped, 0 otherwise */
int skip_string_constants(void)
{
    int c, temp = -1, skip = 0, slash_count = 0;
    while ((c = getch()) != EOF)
    {
        if (c == '\\')
            slash_count++;

        if (c == '"')
            if ((temp != '\\') || (temp == '\\' && ((slash_count % 2) == 0)))
            {
                skip = 1;
                break;
            }

        if (c != '\\')
            slash_count = 0;

        temp = c;
    }
    return skip;
}

/* skip_comments: skip over comments and return 1 if skipped, 0 otherwise */
int skip_comments(void)
{
    int c, temp = -1, skip = 0;

    if ((c = getch()) == '*')
    {
        while ((c = getch()) != EOF)
        {
            if (temp == '*' && c == '/')
            {
                skip = 1;
                break;
            }
            temp = c;
        }
    }
    else if (c == '/')
    {
        while ((c = getch()) != EOF)
            if (c == '\n')
            {
                skip = 1;
                break;
            }
    }
    else
        ungetch(c);

    return skip;
}

/* skip_preprocessor_control_lines: skip over preprocessor control lines and return 1 if skipped, 0 otherwise */
int skip_preprocessor_control_lines(void)
{
    int c, temp = -1, skip = 0;

    while ((c = getch()) != EOF)
    {
        if (temp != '\\' && (c == '\n' || c == EOF))
        {
            skip = 1;
            break;
        }
        temp = c;
    }

    return skip;
}

int skip_character_constants(void)
{
    int c, skip = 0;

    while ((c = getch()) != EOF)
    {
        if (c == '\\')
        {
            c = getch();
            if (c == '\'')
                continue;
        }
        else if (c == '\'')
        {
            skip = 1;
            break;
        }
    }
    return skip;
}

#define BUFSIZE 100

int buf[BUFSIZE]; /* buffer for ungetch*/
int bufp = 0;     /* next free position in buffer */

int getch(void) /* get a (possibly pushed back) character*/
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}