/* \file ogdlbinparser.c

  OGDL Binary format parser. Produces a Graph object.
  
  ogdlbinary ::= (level node )* 0x00
  
  node ::= text_node | binary_node
  text_node ::= utf-8-text-byte* 0x00 
  binary_node ::= 0x01 (length byte[length])* 0x00
  
  where the first node is: 0x01 0x47 0x00
  and level, length are multibyte integers.  
*/

#include "ogdl.h"

static int binary_node(OgdlBinParser);

void OgdlBinParser_graphHandler(OgdlBinParser p, int level, int type, char *s)
{
    int i;
    Graph g;
    
    /* format events ignored */
    if (!type) return;
    
    /* empty nodes are ignored */
    if (!strlen(p->buf)) return;

    /* comments are ignored */
    if (p->buf[0] == '#') return;
    
    if (!p->g) { 
        /* initialize */
        p->g = malloc(sizeof(p->g[0]) * LEVELS);
        if (!p->g) { error(p,ERROR_malloc); return; }
        for (i=1; i<LEVELS; i++)
            p->g[i]=0;
        p->g[0] = Graph_new("_root");
        
        /* The first event is the header, so we return 
         * without adding it to the tree.
         */
         
        return;
    }
    
    /* sanity checks */
    if (level>=(LEVELS-1))   { error(p,ERROR_maxLevels); return; }
    if (level < 0)           { error(p,ERROR_negativeLevels); return; }
    if (p->g[level] == NULL) { error(p,ERROR_nullGraph); return; }

    /* create a new node and add it to current level */
    g = Graph_new(p->buf);
    Graph_addNode(p->g[level],g);
    p->g[level+1]=g;

}

OgdlBinParser OgdlBinParser_new( readFunction readf, int fd )
{
    OgdlBinParser p;
    
    p = (void *) malloc(sizeof(*p));
    if (!p) return NULL;
    
    p->read = readf;
    p->g = 0;
    p->handler = (void *) OgdlBinParser_graphHandler;
    p->errorHandler = 0;
    p->readfd=fd;
    
    return p;		
}

/** Destructor */

void OgdlBinParser_free (OgdlBinParser p)
{
    if (p->g) {
        if (p->g[0])
	    Graph_free(p->g[0]);
	free (p->g);
    }
    
    if (p) 
        free(p);
}

static int read(OgdlBinParser p)
{
	int i= (*(p->read))(p->readfd);
//printf("%x ",i);
return i;	
	
	//return (*(p->read))(p->readfd);
}

static long integer (OgdlBinParser p)
{
    long c;
    
    c = read(p);

    if (c<128) return c;
    return 0; // XXX
}

static int node (OgdlBinParser p)
{
	long i;
	int level, c;
	
	/* read the level */
	p->level = (int) integer(p);
	if (p->level<=0) return 0;
	p->level--;
	
	/* read the first byte: 0x01 introduces binary node */
	c = read(p);
	if ( c <= 0 ) return 0;
	
	if ( c == 1 ) 
	    return binary_node(p);
	
	/* UTF-8 text node (XXX what if not?)
	 * 
	 * A text node is a null terminated array of bytes.
	 * 
	 */
	 
	 i=0;
	 p->buf[i++] = c;
	 while ((c=read(p))>0) {
	 	 if (i > BUFFER) break;	/* XXX */
	     p->buf[i++] = c;
	 }
	 p->buf[i] = 0;
	 p->len = i;
	
	(*p->handler)(p,p->level,EVENT_TEXT,p->buf);

	return 1;
}

static int binary_node (OgdlBinParser p)
{
    /* first byte 0x01 already read */
    
    long len, i;
    int c;
    
    while ( (len = integer(p)) > 0)
    {
	    while (len--) {
	        c=read(p);
	 	 	if (i > BUFFER) break;	/* XXX */
	     	p->buf[i++] = c;
	 	}    		
    }
    
    p->len = i;
    
    /* level is set in node() */
    (*p->handler)(p,p->level,EVENT_BINARY,p->buf);
    
    return 1;
}

Graph OgdlBinParser_parse(OgdlBinParser p)
{
    if (node(p))          /* This is the header. The handler skips this first event */
        while (node(p));

    /* Check that p->g[0] indeed exists */
    return (p && p->g)? p->g[0]:NULL;
}



