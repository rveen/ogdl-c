/** \file xml2ogdl.c

    Converts XML to OGDL.

    Author : Rolf Veen
    Date: 4 Sept 2002
*/

#include <ctype.h>
#include <expat.h>
#include "ogdl.h"

#define BUFFSIZE 8192
#define CONTENT "_"

char buff[BUFFSIZE];
char buf[BUFFSIZE];
char cbuf[BUFFSIZE];

int depth;
int indent = 4;
int flag_content = 0; /* if true, content of elements are identified by CONTENT */

int empty(char *s)
{
    int c;
     
    while ( (c=*s++) ) {
        if (!isspace(c) && !(c==10) && !(c==13)) return 0;
    }
    return 1;
}


/* XXX Mixed content fails !!! */

void chars(void *data, const char *text, int len)
{
    strncat(cbuf,text,len);
}

void start(void *data, const char *el, const char **attr)
{
    int i,j,k;

    cbuf[0] = 0;

    j = depth * indent;
    Graph_printString(el,j,0);
    putchar('\n');

    for (i = 0; attr[i]; i += 2)
    {
    	Graph_printString(attr[i],j+indent,0);
    	putchar('\n');
    	Graph_printString(attr[i+1],j+2*indent,0);
    	putchar('\n');
    }
    depth++;
} 

void end(void *data, const char *el)
{
    if ( ! empty(cbuf) ) {

    	int i;
    	for (i=0; i< depth*indent; i++)
    		putchar(' ');

    	if (flag_content) {
    	    printf(CONTENT " ");
    	}

        Graph_printString (cbuf,0,0);
        putchar('\n');
        cbuf[0] = 0;
    }
    depth--;
}

int main(int argc, char **argv)
{
    int index=1;
    FILE *f = stdin;
    
    XML_Parser p = XML_ParserCreate(NULL);

    if (! p) {
        fprintf(stderr, "Couldn't allocate memory for parser\n");
        exit(-1);
    }

    if (argc==1) {
         puts("usage \\\n  xml2ogdl [-c] [file]");
         puts("version " VERSION );
         puts("options \\\n  -c  identify content by '" CONTENT "'" );
         exit(1);
    }

    while (argc>2) {
        if (!strncmp(argv[index],"-c",2)) {
             flag_content = 1;
             argc-=1;
             index+=1;
        }
    }

    if (argc > 1) {
        f = fopen(argv[index],"r");
        if (!f) {
            printf ("File %s not found\n",argv[index]);
            exit(1);
        }
    }

    XML_SetElementHandler(p, start, end);
    XML_SetCharacterDataHandler(p, chars);
    
    cbuf[0] = 0;

    for (;;) {
        int done;
        int len;

        len = fread(buff, 1, BUFFSIZE, f);
        if (ferror(f)) {
            fprintf(stderr, "read error\n");
            exit(-1);
        }
        done = feof(f);

        if (! XML_Parse(p, buff, len, done)) {
            fprintf(stderr, "parse error at line %d:\n%s\n",
	      XML_GetCurrentLineNumber(p),
	      XML_ErrorString(XML_GetErrorCode(p)));
            exit(-1);
        }

        if (done)
            break;
    }

    return 0;
} 
