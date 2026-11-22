#include <unistd.h>
#include <stdlib.h>

#undef NULL
#define NULL 0
#define EOF (-1)
#define BUFSIZ 1024
#define OPEN_MAX 20 /* max #files open at once */

typedef struct _iobuf
{
    int cnt;    /* characters left*/
    char *ptr;  /* next character position*/
    char *base; /* location of buffer */
    int flag;   /* mode of file access */
    int fd;     /* file descriptor */
} FILE;

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum flags
{
    _READ = 01,  /* file open for reading */
    _WRITE = 02, /* file open for writing */
    _UNBUF = 04, /* file is unbuffered */
    _EOF = 010,  /* end of file */
    _ERR = 020   /* error */
};

FILE _iob[OPEN_MAX] = {
    {0, (char *)0, (char *)0, _READ, 0},
    {0, (char *)0, (char *)0, _WRITE, 1},
    {0, (char *)0, (char *)0, _WRITE | _UNBUF, 2}};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);
int fflush(FILE *);
int fclose(FILE *);

#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc((x), stdout)

#include <fcntl.h>
#define PERMS 0666 /* RW for owner, group and others */

FILE *fopen(const char *name, const char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if ((fp->flag & (_READ | _WRITE)) == 0)
            break;             /* found free slot */
    if (fp >= _iob + OPEN_MAX) /* no free slots */
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a')
    {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    }
    else
        fd = open(name, O_RDONLY, 0);

    if (fd == -1) /* couldn't access name */
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->ptr = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int bufsize;

    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    if (fp->base == NULL)
        if ((fp->base = (char *)malloc(bufsize)) == NULL)
            return EOF;

    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0)
    {
        if (fp->cnt == -1)
            fp->flag |= _EOF;
        else
            fp->flag |= _ERR;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char)*fp->ptr++;
}

/* _flushbuf: flush output buffer */
int _flushbuf(int x, FILE *fp)
{
    int bufsize, n, ub = 0; /* bufsize: size of buffer, n: number of bytes to write, ub: unbuffered flag */
    unsigned char c;

    if ((fp->flag & (_ERR | _EOF | _WRITE)) != _WRITE)
        return EOF;

    c = (unsigned char)x;
    ub = (fp->flag & _UNBUF);    /* set unbuffered flag */
    bufsize = (ub) ? 1 : BUFSIZ; /* 1 for unbuffered, BUFSIZ for buffered */

    if (ub) /* unbuffered */
    {
        if (write(fp->fd, &c, 1) != 1) /* write immediately */
        {
            fp->flag |= _ERR;
            return EOF;
        }
        fp->cnt = 0; /* reset count for unbuffered write */
        return c;    /* return written character for unbuffered write */
    }

    if (fp->base == NULL) /* buffered */
    {
        if ((fp->base = (char *)malloc(bufsize)) == NULL) /* allocate buffer */
        {
            fp->flag |= _ERR;
            return EOF;
        }
    }
    else /* flush existing buffer */
    {
        n = fp->ptr - fp->base;              /* number of bytes to write from buffer */
        if (write(fp->fd, fp->base, n) != n) /* write buffered data */
        {
            fp->flag |= _ERR;
            return EOF;
        }
    }

    fp->ptr = fp->base;            /* reset pointer to start of buffer */
    *fp->ptr++ = (unsigned char)x; /* store new character in buffer */
    fp->cnt = bufsize - 1;         /* set count remaining in buffer */

    return c;
}

static int _fflush_one(FILE *fp)
{
    int n, f = 0;

    if ((fp->flag & (_WRITE | _ERR)) == _WRITE)
    {
        n = (fp->base == NULL || fp->ptr == NULL) ? 0 : fp->ptr - fp->base;
        if (n > 0 && write(fp->fd, fp->base, n) != n)
        {
            fp->flag |= _ERR;
            f |= _ERR;
        }
        fp->ptr = fp->base;
        fp->cnt = 0;
    }
    else
        f |= _ERR;

    return (f != 0) ? EOF : 0;
}

/* fflush: flushes buffer output to output stream */
int fflush(FILE *fp)
{
    int f = 0;
    FILE *ifp;

    if (fp == NULL)
        for (ifp = _iob; (ifp - _iob) < OPEN_MAX; ifp++)
            f |= (ifp->flag & _WRITE) ? _fflush_one(ifp) : 0;
    else
        f |= _fflush_one(fp);

    return (f != 0) ? EOF : 0;
}

int fclose(FILE *fp)
{
    int f = 0, s = 0;

    if (fp == NULL || (fp->flag & (_WRITE | _READ)) == 0)
        return EOF;

    if ((fp->flag & (_WRITE)) == _WRITE)
        f = fflush(fp);

    if (fp->base != NULL)
        free(fp->base);

    s = close(fp->fd);

    fp->base = NULL;
    fp->ptr = NULL;
    fp->cnt = 0;
    fp->flag = 0;
    fp->fd = -1;

    return (f != 0 || s != 0) ? EOF : 0;
}

int main(void)
{
    FILE *fp;

    if ((fp = fopen("/tmp/futil_test.txt", "r")) == NULL)
        return 2;

    int c;

    while ((c = getc(fp)) != EOF)
        putchar(c);

    fflush(NULL);

    if (ferror(fp))
        return 1;

    fclose(fp);
    return 0;
}