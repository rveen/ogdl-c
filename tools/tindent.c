/** \file tindent.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int c, i=-1, n=2, state=1;

    if (argc > 1) {
        if (!strncmp(argv[1],"-h",2) || ! strncmp(argv[1],"--help",6)) {
            puts("usage: tindent [n] [title]");
            exit(1);
        }
        n = atoi(argv[1]);
    }
    
    if (argc > 2)
        puts(argv[2]);

    while ((c = getchar()) != EOF) {
        if (c != '\r') {
            if (state) {
                for (i=0; i<n; i++)
                    putchar(' ');
                state=0;
            }
            putchar(c);
        }
        if (c == '\n') 
            state=1;
    }

    return 0;
}
