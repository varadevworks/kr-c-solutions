#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nlist /* table entry: */
{
    struct nlist *next; /* next entry in chain */
    char *name;
    char *defn;
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s);
struct nlist *lookup(char *s);
int undef(char *name);
struct nlist *install(char *name, char *defn);

int main(void)
{
    install("name1", "definition1");
    install("name2", "definition2");
    undef("name1");
    struct nlist *np = lookup("name2");
    if (np != NULL)
        printf("%s: %s\n", np->name, np->defn);
    np = lookup("name1");
    if (np != NULL)
        printf("%s: %s\n", np->name, np->defn);
    else
        printf("name1 not found\n");
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
        if (strcmp(s, np->name) == 0)
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