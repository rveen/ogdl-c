/** \file ogdllog.c
   
   Appends OGDL graphs to a file.
   
   R.Veen, Jan 2004.
*/

#include <stdlib.h>
#include <stdio.h>
#include "ogdl.h"

/** The constructor */

OgdlLog OgdlLog_new(char *fileName)
{
    OgdlLog l;
    FILE *f;
    
    f = fopen(fileName,"a+");
    if (!f) {
        fprintf(stderr,"OgdlLog_new(): cannot open %s mode a+\n",fileName); 
        return 0;
    }
    
    l = (void *) malloc(sizeof(*l));
    if (!l) return 0;
   
    l->f = f;
    l->p = 0;
    return l;
}

/** The destructor */

void OgdlLog_free (OgdlLog l)
{
    if (!l) return;
    
    if (l->p)
        OgdlParser_free(l->p);
	
    fclose(l->f);
    free(l);
}

/** append a graph to a file, in OGDL. 

    It returns the position at which the
    graph starts
*/

unsigned long OgdlLog_add (OgdlLog l, Graph g)
{
    unsigned long j;
    
    j = ftell(l->f);	/* should use ftello */
    Graph_fprint(g,l->f,-1,1,1);
    fputc(OGDL_EOS,l->f);
    fputc('\n',l->f);	/* for readability */
    fflush(l->f);
    return j;
}

/** get a graph from an OGDL log file */

Graph OgdlLog_get (OgdlLog l, unsigned long offset)
{
    Graph g=0;

    if ( fseek(l->f,offset,SEEK_SET) ) return 0;
   
    if (!l->p)
        l->p = OgdlParser_new();
    else if (l->p->g) {
         if (l->p->g[0])
	     free (l->p->g[0]);
	 free(l->p->g);
	 l->p->g = 0;
    }

    OgdlParser_parse(l->p,l->f);
      
    if (l->p->g)
        g = l->p->g[0];

    return g;
}

/** get the next graph from an OGDL log file */

Graph OgdlLog_next (OgdlLog l)
{
    Graph g=0;
    int c;
    
    if (!l) return 0;
 
    /* tolerate a stream beginning with EOS */

    c = getc(l->f);
    if (c == EOF) return 0;		/* XXX needed cos of bug in OgdlParser ? */

    if (c != OGDL_EOS)
        ungetc(c,l->f);
    
    if (!l->p)
        l->p = OgdlParser_new();
    else
        l->p = OgdlParser_reuse(l->p);
	
    OgdlParser_parse(l->p,l->f);
    
    if (l->p->g)
        g = l->p->g[0];

    return g;
}

/** get the current position in the log file */

unsigned long OgdlLog_position(OgdlLog l)
{
    return ftell(l->f);
}
