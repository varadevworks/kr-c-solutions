#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define MAX_PATH 1024

void fsize(char *);
void dirwalk(char *, void (*fcn)(char *));
char *format_time(time_t *t);

typedef struct dirent Dirent;

int main(int argc, char *argv[])
{
    if (argc == 1) /* default: current directory */
        fsize(".");
    else
        while (--argc > 0)
            fsize(*++argv);

    return 0;
}

/* fsize: print the name of file "name" and its size */
void fsize(char *name)
{
    struct stat stbuf;

    if (stat(name, &stbuf) == -1)
    {
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;
    }

    if (S_ISDIR(stbuf.st_mode))
        dirwalk(name, fsize);

    printf("%lld %s\n", (long long)stbuf.st_size, name);
    printf("\tdevice id: %d | ", stbuf.st_dev);
    printf("mode : %#o | ", stbuf.st_mode);
    printf("number of links: %hu | ", stbuf.st_nlink);
    printf("inode number: %llu | ", (unsigned long long)stbuf.st_ino);
    printf("user id: %u | ", stbuf.st_uid);
    printf("group id: %u | ", stbuf.st_gid);
    printf("device id(rdev): %d | ", stbuf.st_rdev);
    printf("access time: %s | ", format_time(&stbuf.st_atimespec.tv_sec));
    printf("modified time: %s | ", format_time(&stbuf.st_mtimespec.tv_sec));
    printf("change time: %s | ", format_time(&stbuf.st_ctimespec.tv_sec));
    printf("create time: %s | ", format_time(&stbuf.st_birthtimespec.tv_sec));
    printf("block count: %lld | ", (long long)stbuf.st_blocks);
    printf("block size: %d |\n", stbuf.st_blksize);
}

/* dirwalk: apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char *))
{
    char name[MAX_PATH];
    Dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(dir)) == NULL)
    {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }

    while ((dp = readdir(dfd)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue; /* skip current and parent directories */
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
        {
            fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
            continue;
        }
        else
        {
            sprintf(name, "%s/%s", dir, dp->d_name);
            (*fcn)(name);
        }
    }
    closedir(dfd);
}

char *format_time(time_t *t)
{
    static char buffer[26];
    char *s = ctime(t);
    if (s == NULL)
        return "N/A";
    strncpy(buffer, s, sizeof(buffer));
    buffer[24] = '\0'; /* remove newline added by ctime */
    return buffer;
}
/*
On macOS, struct stat contains the following fields (when _DARWIN_FEATURE_64_BIT_INODE is defined)
    dev_t st_dev;                     - ID of device containing file
    mode_t st_mode;                   -  Mode of file
    nlink_t st_nlink;                 -  Number of hard links
    ino_t st_ino;                     - File serial number
    uid_t st_uid;                     - User ID of the file
    gid_t st_gid;                     - Group ID of the file
    dev_t st_rdev;                    - Device ID
    struct timespec st_atimespec;     - Time of last access
    struct timespec st_mtimespec;     - Time of last data modification
    struct timespec st_ctimespec;     - Time of last status change
    struct timespec st_birthtimespec; - Time of file creation (birth)
    off_t st_size;                    - File size, in bytes
    blkcnt_t st_blocks;               - Blocks allocated for file
    blksize_t st_blksize;             - Optimal block size for I/O
    uint32_t st_flags;                - User-defined flags for file
    uint32_t st_gen;                  - File generation number
*/