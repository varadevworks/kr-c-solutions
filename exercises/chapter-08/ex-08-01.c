#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFSIZE 4096

int main(int argc, char *argv[])
{
    int cat(const int sfd, int dfd);
    int fd, e = 0;

    if (argc == 1)
    {
        if (cat(STDIN_FILENO, STDOUT_FILENO) == -1)
            e |= 1;
        return e;
    }

    while (--argc > 0)
        if ((fd = open(*++argv, O_RDONLY, 0)) == -1)
        {
            fprintf(stderr, "cat: can't open %s\n", *argv);
            e |= 1;
        }
        else
        {
            if (cat(fd, STDOUT_FILENO) == -1)
                e |= 1;
            close(fd);
        }

    return e;
}

/* cat: copy from sfd to dfd, return 0 on success, -1 on write error */
int cat(const int sfd, int dfd)
{
    int n = 0;
    char buf[BUFSIZE];

    while ((n = read(sfd, buf, BUFSIZE)) > 0)
        if (write(dfd, buf, n) != n)
        {
            fprintf(stderr, "cat: write error\n");
            return -1;
        }
    if (n == -1)
        fprintf(stderr, "cat: read error\n");
    return (n == -1 ? -1 : 0);
}