#include <unistd.h>
#include <stdlib.h>

#undef NULL

#define NULL 0
#define EOF -1
#define BUFSIZ 1024
#define OPEN_MAX 20 /* maximum number of open files */

struct flags
{
    unsigned int read : 1;
    unsigned int write : 1;
    unsigned int unbuf : 1;
    unsigned int err : 1;
    unsigned int eof : 1;
};

typedef struct _iobuf
{
    int cnt;    /* number of characters left */
    char *ptr;  /* next character position */
    char *base; /* location of the buffer */
    struct flags flags;
    int fd; /* file descriptor*/
} FILE;

FILE _iob[OPEN_MAX] = {
    {0, (char *)NULL, (char *)NULL, {1, 0, 0, 0, 0}, 0}, /* stdin */
    {0, (char *)NULL, (char *)NULL, {0, 1, 0, 0, 0}, 1}, /* stdout */
    {0, (char *)NULL, (char *)NULL, {0, 1, 1, 0, 0}, 2}  /* stderr */
};

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p) ((p)->flags.eof)
#define ferror(p) ((p)->flags.err)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc((x), stdout)

#include <fcntl.h>
#define PERMS 0666

FILE *fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if (!fp->flags.read && !fp->flags.write)
            break; /* found free slot */

    if (fp >= _iob + OPEN_MAX)
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a')
    {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, SEEK_END);
    }
    else
        fd = open(name, O_RDONLY, 0);

    if (fd == -1)
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flags.read = (*mode == 'r') ? 1 : 0;
    fp->flags.write = (*mode != 'r') ? 1 : 0;
    fp->flags.unbuf = 0;
    fp->flags.eof = 0;
    fp->flags.err = 0;

    return fp;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int bufsize;

    if (!fp->flags.read || fp->flags.err || fp->flags.eof)
        return EOF;

    bufsize = (fp->flags.unbuf) ? 1 : BUFSIZ;
    if (fp->base == NULL)
        if ((fp->base = (char *)malloc(bufsize)) == NULL)
            return EOF;

    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0)
    {
        if (fp->cnt == -1)
            fp->flags.eof = 1;
        else
            fp->flags.err = 1;
        fp->cnt = 0;
        return EOF;
    }

    return (unsigned char)*fp->ptr++;
}

int main(void)
{
    int c;
    char v;
    FILE *fp;

    fp = fopen("/tmp/futil_test.txt", "r");

    while (fp != NULL && (c = getc(fp)) != EOF)
    {
        v = (unsigned char)c;
        write(1, &v, 1);
    }

    return 0;
}