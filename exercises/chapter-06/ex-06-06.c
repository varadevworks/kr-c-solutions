
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum token_type
{
    COMMENT,
    LITERAL,
    PREPROCESSOR,
    IDENTIFIER,
    OTHER
};

struct nlist /* table entry: */
{
    struct nlist *next; /* next entry in chain */
    char *name;
    char *defn;
    int replacing;
};

#define HASHSIZE 101
#define MAXWORD 10000
#define NPLISTSIZE 100

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s);
struct nlist *lookup(char *s);
int undef(char *name);
struct nlist *install(char *name, char *defn);
int getword(char *word, int lim);
int getch(void);
void ungetch(int c);
int parse_define_preprocessor(char *word, char *key, char *value);

int main(void)
{
    char word[MAXWORD];
    int type;
    struct nlist *np;
    char key[MAXWORD];
    char value[MAXWORD];
    struct nlist *nplist[NPLISTSIZE]; /* store pointers recent macro expansions */
    int nplist_index = 0;

    while ((type = getword(word, MAXWORD)) != EOF)
        if (type == OTHER && word[0] == '\x02' && (nplist_index - 1) >= 0 && nplist[nplist_index - 1]->replacing) /* identified recursive macro marker */
            nplist[--nplist_index]->replacing = 0;
        else if (type == IDENTIFIER && (np = lookup(word)) != NULL) /* push the definition back onto the input stream for recursive macro expansion */
        {

            char *d = np->defn + strlen(np->defn);
            np->replacing = 1; /* set the replacing flag */
            ungetch('\x02');   /* push a marker onto the input stream */
            while (d != np->defn)
                ungetch(*--d);
            if (nplist_index < NPLISTSIZE)
                nplist[nplist_index++] = np; /* store pointer to recent macro expansion */
        }
        else if (type == PREPROCESSOR && parse_define_preprocessor(word, key, value))
            install(key, value);
        else
            printf("%s", word);

    return 0;
}

/* hash: form hash value for a string */
unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0 && !np->replacing)
            return np; /* found */
    return NULL;       /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) /* not found */
    {
        np = (struct nlist *)malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        np->replacing = 0;
        hashtab[hashval] = np;
    }
    else                        /* already there*/
        free((void *)np->defn); /* free previous defn */

    if ((np->defn = strdup(defn)) == NULL)
        return NULL;

    return np;
}

/* undef: remove (name, defn) from hashtab */
int undef(char *name)
{
    unsigned hashval = hash(name); /* hashvalue of name */
    struct nlist *curr, *prev;     /* current, previous pointer */

    for (prev = NULL, curr = hashtab[hashval]; curr != NULL; prev = curr, curr = curr->next)
        if (strcmp(curr->name, name) == 0) /* match found */
        {
            if (prev == NULL) /* first element in chain */
                hashtab[hashval] = curr->next;
            else /* not the first element in chain */
                prev->next = curr->next;
            free((void *)curr->name);
            free((void *)curr->defn);
            free((void *)curr);
            return 1;
        }

    return 0;
}

int getword(char *word, int lim)
{
    int c;
    int getch(void);
    void ungetch(int c);
    int extract_comment(char *word, int lim);
    int extract_preprocessor(char *word, int lim);
    int extract_string_literal(char *word, int lim);
    int extract_character_literal(char *word, int lim);
    int extract_identifier(char *word, int lim);
    int extract_other(char *word, int lim);

    char *w = word;

    if ((c = getch()) == EOF)
        return EOF;
    else
        *w++ = c; /* store the current character */

    if (c == '/')
        return extract_comment(w, --lim);
    else if (c == '#')
        return extract_preprocessor(w, --lim);
    else if (c == '"')
        return extract_string_literal(w, --lim);
    else if (c == '\'')
        return extract_character_literal(w, --lim);
    else if (isalpha(c) || c == '_')
        return extract_identifier(w, --lim);
    else
        return extract_other(w, --lim);
}

/* extract_comment: extract a // or block comment until its end. */
int extract_comment(char *word, int lim)
{
    int c, prev = -1, s = 1; /* prev - previous character, s - indicates if comment is extracted: 0 no comment, 1 comment*/
    char *w = word;

    if ((c = getch()) == '/') /* check for '//' */
    {
        *w++ = c; /* store the second '/' */
        while (--lim > 0 && (c = getch()) != EOF && c != '\n')
            *w++ = c;
    }
    else if (c == '*') /* check for block comment */
    {
        *w++ = c; /* store the '*' */
        while (--lim > 0 && (c = getch()) != EOF)
        {
            *w++ = c;
            if (prev == '*' && c == '/') /* end of block comment */
                break;
            prev = c;
        }
    }
    else /* not a '//'  or block comment */
    {
        ungetch(c); /* push back the character as it is not part of a comment */
        s = 0;      /* no comment extracted */
    }

    if (s && c == '\n')
        ungetch(c); /* push back EOF or newline */

    *w = '\0';
    return s ? COMMENT : OTHER;
}

/* extract_preprocessor: extract a preprocessor directive until its end. */
int extract_preprocessor(char *word, int lim)
{
    int c, prev = -1;
    char *w = word;

    while (--lim > 0 && (c = getch()) != EOF)
    {
        *w++ = c;                      /* store the current character */
        if (prev != '\\' && c == '\n') /* end of preprocessor directive */
            break;
        prev = c;
    }

    *w = '\0';
    return PREPROCESSOR;
}

/* extract_string_literal: extract a string literal until its end. */
int extract_string_literal(char *word, int lim)
{
    int c, prev = -1, s = 0; /* prev - previous character, s - slash count */
    char *w = word;

    while (--lim > 0 && (c = getch()) != EOF)
    {
        *w++ = c; /* store the current character */

        if (c == '\\')
            s++;

        if (c == '"' && ((prev != '\\') || (prev == '\\' && ((s % 2) == 0)))) /* end of string literal */
            break;

        if (c != '\\')
            s = 0;

        prev = c;
    }

    *w = '\0';
    return LITERAL;
}

/* extract_character_literal: extract a character literal until its end. */
int extract_character_literal(char *word, int lim)
{
    int c, prev = -1, s = 0; /* prev - previous character, s - slash count */
    char *w = word;

    while (--lim > 0 && (c = getch()) != EOF)
    {
        *w++ = c; /* store the current character */
        if (c == '\\')
            s++;
        if ((prev != '\\' || (prev == '\\' && (s % 2 == 0))) && c == '\'') /* end of character literal */
            break;
        if (c != '\\')
            s = 0;

        prev = c;
    }

    *w = '\0';
    return LITERAL;
}

/* extract_identifier: extract an identifier until its end. */
int extract_identifier(char *word, int lim)
{
    int c;
    char *w = word;

    while (--lim > 0 && (c = getch()) != EOF) /* extract identifier */
        if (!isalnum(c) && c != '_')          /* not part of identifier */
        {
            ungetch(c);
            break;
        }
        else
            *w++ = c;

    *w = '\0';
    return IDENTIFIER;
}

/* extract_other: extract any other sequence of characters until its end. */
int extract_other(char *word, int lim)
{
    int c;
    char *w = word;

    if (*(w - 1) != '\x02') /* if the previous character is not the recursive macro marker */
        while (--lim > 0 && (c = getch()) != EOF)
        {
            if (isalnum(c) || c == '_' || c == '"' || c == '\'' || c == '#' || c == '/' || c == '\x02') /* end of 'other' */
            {
                ungetch(c); /* push back the character that ends 'other' */
                break;      /* end of 'other' */
            }
            else
                *w++ = c;
        }

    *w = '\0';
    return OTHER;
}

#define MAX_KEY_LENGTH 101
#define MAX_VALUE_LENGTH 1001

/* parse_define_preprocessor: parse a #define preprocessor directive and extract its key and value. */
int parse_define_preprocessor(char *word, char *key, char *value)
{
    int p = 0, i, j, k; /* define preprocessor exists: 1 - yes, 0 - no */
    char *w = word;
    char dp[7]; /* buffer for storing "define" */

    while (*w != '\0' && (*w == '#' || *w == ' ' || *w == '\t')) /*skip leading # and whitespace*/
        w++;

    /* get the "define" keyword */
    for (i = 0; i < 6 && *w != '\0' && isalpha(*w); w++, i++) /* extract key */
        dp[i] = *w;
    dp[i] = '\0';

    if (strcmp(dp, "define") == 0) /* check if the key is "define" */
    {
        p = 1;
        /* skip whitespaces */
        while (*w != '\0' && (*w == ' ' || *w == '\t'))
            w++;

        /* extract identifier (macro name) */
        for (k = 0; k < MAX_KEY_LENGTH - 1 && *w != '\0' && (isalnum(*w) || *w == '_'); w++, k++)
            key[k] = *w;
        key[k] = '\0';

        /* skip whitespaces before the replacement text */
        while (*w != '\0' && (*w == ' ' || *w == '\t'))
            w++;

        /* extract replacement text of define*/
        for (j = 0; j < MAX_VALUE_LENGTH - 1 && *w != '\0'; w++) /* extract value */
            if (*w == '\\' && *(w + 1) == '\n')                  /* handle backslash-newline combination */
                w++;                                             /* skip the backslash and newline */
            else
                value[j++] = *w;

        /* remove trailing whitespaces from the value */
        for (k = j - 1; k >= 0 && (value[k] == '\n' || value[k] == ' ' || value[k] == '\t'); k--)
            ;
        value[++k] = '\0';
    }

    // printf("Parsed define: key = %s, value = %s\n", key, value);

    return p;
}

#define BUFSIZE 10000

static char buf[BUFSIZE]; /* buffer for ungetch*/
static int bufp = 0;      /* next free position in buffer */

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