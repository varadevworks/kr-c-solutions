#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINELEN 1001

int compare_files(const char *filename1, const char *filename2);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        exit(2);
    }

    exit(compare_files(argv[1], argv[2]));
}

/* compare_files: Compare two files line by line
 * Returns:
 *      0 if files are identical
 *      1 if they differ
 *      2 on error.
 */
int compare_files(const char *filename1, const char *filename2)
{
    char l1[MAXLINELEN];
    char l2[MAXLINELEN];
    FILE *fp1, *fp2;

    fp1 = fopen(filename1, "r");
    if (fp1 == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename1);
        return 2;
    }
    fp2 = fopen(filename2, "r");
    if (fp2 == NULL)
    {
        fclose(fp1);
        fprintf(stderr, "Error: Could not open file %s\n", filename2);
        return 2;
    }

    char *rp1, *rp2;
    int ret = 0; /* return value, 0 if identical, 1 if different, 2 if error */

    while (1)
    {
        l1[0] = '\0';
        l2[0] = '\0';

        rp1 = fgets(l1, sizeof(l1), fp1);
        rp2 = fgets(l2, sizeof(l2), fp2);

        if (rp1 == NULL && rp2 == NULL) /* End of both files */
            break;

        if (rp1 == NULL && rp2 != NULL) /* End of file1 but not file2 */
        {
            ret = 1;
            printf("Difference found:\n");
            printf("File2: %s", l2);
            break;
        }

        if (rp1 != NULL && rp2 == NULL) /* End of file2 but not file1 */
        {
            ret = 1;
            printf("Difference found:\n");
            printf("File1: %s", l1);
            break;
        }

        if (strcmp(l1, l2) != 0) /* first difference found */
        {
            ret = 1;
            printf("Difference found:\n");
            printf("File1: %s", l1);
            printf("File2: %s", l2);
            break;
        }
    }

    if (ferror(fp1) || ferror(fp2)) /* Check for read errors */
    {
        ret = 2;
        fprintf(stderr, "Error: Reading files: %s and %s\n", filename1, filename2);
    }

    fclose(fp1);
    fclose(fp2);
    return ret;
}