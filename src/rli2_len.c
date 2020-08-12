#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.c"

/**
 * Name...: rli2_len.c
 * Author.: Xanadrel
 * Desc...: Same as rli2 but using line length from infile for the comparison
 * License.: MIT
 */

static int cmp_cache(const void *p1, const void *p2, const int len)
{
    return strncmp(p1, p2, len);
}

int main(int argc, char *argv[])
{
    FILE *fd1;
    FILE *fd2;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s infile removefile\n", argv[0]);

        return (-1);
    }

    char *infile = argv[1];
    char *removefile = argv[2];

    if ((fd1 = fopen(infile, "rb")) == NULL)
    {
        fprintf(stderr, "%s: %s\n", infile, strerror(errno));

        return (-1);
    }

    if ((fd2 = fopen(removefile, "rb")) == NULL)
    {
        fprintf(stderr, "%s: %s\n", removefile, strerror(errno));

        return (-1);
    }

    int len;
    char line_buf1[BUFSIZ];
    char line_buf2[BUFSIZ];

    // Read first line of each file
    if ((len = fgetl(fd1, BUFSIZ, line_buf1)) == -1) memset(line_buf1, 0, BUFSIZ);
    if (fgetl(fd2, BUFSIZ, line_buf2) == -1) memset(line_buf2, 0, BUFSIZ);

    int comp;

    while (!feof(fd1) && !feof(fd2))
    {
        comp = cmp_cache(line_buf1, line_buf2, len);

        if (comp == 0)
        {
            puts(line_buf2);

            // Read a line from both files
            if ((len = fgetl(fd1, BUFSIZ, line_buf1)) == -1) memset(line_buf1, 0, BUFSIZ);
            if (fgetl(fd2, BUFSIZ, line_buf2) == -1) memset(line_buf2, 0, BUFSIZ);
        }
        else if (comp > 0)
        {
            if (fgetl(fd2, BUFSIZ, line_buf2) == -1) memset(line_buf2, 0, BUFSIZ);
        }
        else if (comp < 0)
        {
            if ((len = fgetl(fd1, BUFSIZ, line_buf1)) == -1) memset(line_buf1, 0, BUFSIZ);
        }
    }

    fclose(fd1);
    fclose(fd2);

    return 0;
}