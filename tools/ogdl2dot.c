/** \file ogdl2dot.c

    Prints an OGDL tree to dot format

    - author    Rolf Veen
    - date      20031126
    - license   zlib
    - see       http://ogdl.sf.net
*/

#include <ctype.h>

#include "ogdl.h"

/*
static void putstr(char *s)
{
    register char *p, c;
    
    p = s;
    while (c=*p++)
        putchar(c);
}
*/

int N=0;

void _ogdl2dot(Graph g, int up)
{
    int i, j;
    Graph node;
    char *p;
    
    j=++N;
    printf("  %d [ label = \"",N);
    
    /* transform ASCII 0x10 into \n */
    p = g->name;
    while ( (i=*p++) ) {
        if (i=='\n') {
	    putchar('\\');
	    putchar('n');
	}
	else
	    putchar(i);
    }
    
    puts("\" ];");
    if (N>1) printf("  %d -> %d;\n",up,N);
    
    for (i=0; i<g->size; i++) {
        node = g->nodes[i];
        _ogdl2dot(node,j);
    }
}

void usage()
{
        puts("ogdl2dot 'OGDL to dot'");
        puts("usage \\\n  ogdl2dot [-d N] [-r] file");
        puts("version 0.0");
        exit(1);
}

int main(int argc, char **argv)
{
    Graph g;
    FILE *f = stdin;
    OgdlParser parser;
    int index=1, maxLevel=-1, root=0;

    if (argc>1)
    while (1) {
      if (!strncmp(argv[index],"-d",2)) {
        maxLevel = atoi(argv[index+1]);
        argc-=2;
        index+=2;
      }
      else if (!strcmp(argv[index],"-r")) {   
        argc--;
        index++;
        root = 1;
      }
      else if (!strcmp(argv[index],"-help")) 
        usage();
      else 
        break;
    }

    if (argc > 1) {
        f = fopen(argv[index],"r");
        if (!f) {
            printf ("File %s not found\n",argv[index]);
            exit(1);
        }
    }
    
    parser = OgdlParser_new();
    OgdlParser_parse(parser,f);
    fclose(f); 

    if (! parser->g) exit(1);
    if (! parser->g[0]) exit(1);
    g = parser->g[0];
    if (!g) return 1;

    puts("digraph _name_ {\n");
    puts("  rankdir=\"LR\";\n  node [ color = \"#ff8000\" ];\n  edge [ color = \"#cccccc\" ];");

    _ogdl2dot(g,0);
    puts("}");

    OgdlParser_free(parser);

    exit(0);
}

