/** \file gpath.c

    Path evaluator for OGDL data streams

    Paths follow the OGDL Path specification, with
    the following exception:

    - '.' matches the compleet tree, from the base on.

    author: Rolf Veen
    first release: 20020902 (see Changelog)
    license: zlib
    see: http://ogdl.org
*/

#include "ogdl.h"

void putstr(char *s)
{
    int c=0;
    
    while (*s)
        putchar(c=*s++);

    if (c!='\n')
        putchar('\n');
}

static int fbasename(char *d, char *s)
{
    char *p, b[128];

    strncpy(b,s,127);
    b[127]=0;
    
    p=b+127;
    while (p>b) {
        if (*p=='.') {
            *p=0;
            break;
        }
        p--;
    }
    p=b+127;
    while (p>b) {
        if (*p=='/') {
            *p=0;
            p++;
            break;
        }
        p--;
    }
    strcpy(d,p);

    return 0;
}

int main(int argc, char **argv)
{
    char *path, rootstr[128];
    Graph g;
    FILE *f = stdin;
    OgdlParser parser;
    int index=1, maxLevel=-1, indent=2, root=0;

    if (argc==1) {
        puts("gpath 'OGDL path resolver'");
        puts("usage \\\n  gpath [-d depth] [-n indent] [-r] <path> [file]");
        puts("version " VERSION);
        exit(1);
    }

    while (1) {
      if (!strncmp(argv[index],"-d",2)) {
        maxLevel = atoi(argv[index+1]);
        argc-=2;
        index+=2;
      }
      else if (!strncmp(argv[index],"-n",2)) {
        indent = atoi(argv[index+1]);
        argc-=2;
        index+=2;
        if (indent == 0) indent = 1;
      }
      else if (!strcmp(argv[index],"-r")) {   
        argc--;
        index++;
        root = 1;
      }
      else 
        break;
    }
    // first non option is path (mandatory)
    path = argv[index++];
    argc--;

    if (argc > 1) {
        f = fopen(argv[index],"r");
        if (!f) {
            printf ("File %s not found\n",argv[index]);
            exit(1);
        }
        /* set the root text to the file name */
        fbasename(rootstr,argv[index]);
    }
    else
        strcpy(rootstr,"root");
   
    parser = OgdlParser_new();
    OgdlParser_parse(parser,f);
    fclose(f); 

    if (! parser->g || ! parser->g[0]) 
        exit(1);
    g = parser->g[0];

    Graph_setName(g,rootstr);

    if (strcmp(".",path)) 
         g = Graph_get(g,path);

    if (g) {
        if (root) 
            Graph_fprint(g,stdout,maxLevel,indent,0);
        else if (g->size == 0 )
            putstr(g->name);
        else if (g->size == 1 && !g->nodes[0]->size)
            putstr(g->nodes[0]->name);
        else
            Graph_fprint(g,stdout,maxLevel,indent,1);
    }

    OgdlParser_free(parser);
    exit(g?0:1);
}

