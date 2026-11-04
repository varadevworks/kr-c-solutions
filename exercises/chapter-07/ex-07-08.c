#include <stdio.h>
#include <stdlib.h>

#define MAXLINESPERPAGE 60

int print_file(char *filename);

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s filename1 [filename2 ...]\n", argv[0]);
        exit(1);
    }

    int r = 0; /* return value to track success or failure of printing files */
    while (--argc > 0)
        r |= print_file(*++argv); /* print each file and update return value if any error occurs */

    exit(r);
}

/* print_file: print the contents of a file with page headers.
 * return: 0 if successful, 1 if an error occurred
 */
int print_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return 1;
    }

    int page_no = 1, prev_page = 0; /* page_no: current page number , prev_page: previous page number */
    int line_no = 1;                /* line_no: current line number on the page */
    int c, p = 0, r = 0;            /* c: current character, p: previous character, r: return value */

    printf("----- filename: %s ------- Page No: %d\n", filename, page_no); /* print header for the first page */

    while ((c = fgetc(fp)) != EOF)
    {
        if (page_no != 1 && page_no != prev_page) /* when page number changes */
        {
            if (p != '\f') /* if the previous character is not a form feed, print a form feed */
                putchar('\f');
            printf("----- filename: %s ------- Page No: %d\n", filename, page_no); /* print header for the new page */
            prev_page = page_no;                                                   /* update previous page number */
            line_no = 1;                                                           /* reset line number for the new page */
        }

        putchar(c);
        if (c == '\n') /* increment line number on the page for each newline */
            line_no++;

        if (c == '\f' || line_no >= MAXLINESPERPAGE) /* increment page number when form feed or max lines reached */
            page_no++;

        p = c; /* update previous character */
    }

    if (p != '\f' && line_no <= MAXLINESPERPAGE) /* print a form feed at the end of the last page if not already done */
        putchar('\f');

    if (ferror(fp))
    {
        fprintf(stderr, "Error reading file %s\n", filename);
        r = 1;
    }

    fclose(fp);
    return r;
}
