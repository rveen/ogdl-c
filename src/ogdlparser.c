/** \file ogdlparser.c

    Non-recursive OGDL parser: character stream to events.
    
    Needs only one consecutive ungetc().
    
    Usefull for 8-bit streams that are ASCII transparent,
    such as plain ASCII, ISO-? variants and UTF-8.
    
    TO DO:
      - fine grained events (related to formatting)
       
    Author: R. Veen
    Date: 26 Oct 2002
    Modified: see Changelog
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ogdl.h"

static int line(OgdlParser p);

static int error(OgdlParser p, int n)
{
    (*p->errorHandler)(p,n);
    return 0;
}

const char * OgdlParser_getErrorMessage(int n)
{
    switch (n)
    {
        case ERROR_none:
           return("");
        case ERROR_malloc:
            return("malloc() returns null in graphHandler");
        case ERROR_maxLevels:
            return("MAXLEVELS reached in graphHandler()");
        case ERROR_negativeLevels:
            return("level < 0 in graphHandler()");
        case ERROR_nullGraph:
            return("graph at current level is null in graphHandler()");
        case ERROR_mixedTabsSpaces:
            return("mixed TABS and SPACES in indentation");
        case ERROR_textOverflow1:
            return("text buffer overflow at word() [1]");    
        case ERROR_textOverflow2:
            return("text buffer overflow at word() [2]");
        case ERROR_textOverflow3:
            return("text buffer overflow at quoted() [1]");
        case ERROR_textOverflow4:
            return("text buffer overflow at quoted() [2]"); 
        case ERROR_textOverflow5:
            return("text buffer overflow at block() [1]");
        case ERROR_textOverflow6:
            return("text buffer overflow at block() [2]");
        case ERROR_textOverflow7:
            return("text buffer overflow at block() [3]");
        case ERROR_textOverflow8:
            return("text buffer overflow at block() [4]");      
        case ERROR_maxGroups:
            return("MAXGROUPS reached");
        case ERROR_negativeGroups:
            return("groupIndex going to -1");
        case ERROR_maxLevels2:
            return("line(): max LEVELS reached");
        case ERROR_notFound:
            return("Not found");
        case ERROR_realloc:
            return("realloc() failed");
        case ERROR_argumentOutOfRange:
            return("Argument out of range");
        case ERROR_noObject:
            return("No object");
        case ERROR_argumentIsNull:
            return("Null argument exception");
        default:
            return("Unknown error");
    }
    return("");
} 

void OgdlParser_error(OgdlParser p, int n)
{
    printf("ogdlparser.c: [ERR %d] %s at line %d\n", 
           n,OgdlParser_getErrorMessage(n),p->line);
    exit(1);
}

/** Constructor. Returns a pointer to a newly allocated memory structure */

OgdlParser OgdlParser_new()
{
    OgdlParser p;
    
    p = (void *) malloc(sizeof(*p));
    if (!p) return NULL;
    
    p->level = 0;
    p->line_level = 0;
    p->saved_space = 0;
    p->saved_newline = 0;
    p->indentation[0] = 0;
    p->groupIndex = 0;
    p->g = 0;
    p->handler = (void *) OgdlParser_graphHandler;
    p->errorHandler = (void *) OgdlParser_error;

    p->tabs=-1;
    p->line=0;
    p->is_comment = 0;
    
    return p;
}

OgdlParser OgdlParser_detachGraph(OgdlParser p)
{
    if (!p) return 0;
  
    if ((p->g) && (p->g[0])) 
    {
        p->g[0] = NULL;
    }
    
    return OgdlParser_reuse(p);
}
OgdlParser OgdlParser_reuse(OgdlParser p)
{
    if (!p) return 0;
  
    p->level = 0;
    p->line_level = 0;
    p->saved_space = 0;
    p->saved_newline = 0;
    p->indentation[0] = 0;
    p->groupIndex = 0;

    if (p->g) {
        if (p->g[0])
	    Graph_free(p->g[0]);
	free (p->g);
    }

    p->g = 0;  	                      
    /* p->handler: maintain the same */
    p->tabs=8;
    p->line=0;
    p->is_comment = 0;
    
    return p;
}

/** Destructor */

void OgdlParser_free (OgdlParser p)
{
    if (p->g) {
        if (p->g[0])
	    Graph_free(p->g[0]);
	free (p->g);
    }
    
    if (p) 
        free(p);
}

/** Changes the default event handler of the parser */

void OgdlParser_setHandler (OgdlParser p, eventHandlerFunction h)
{
    p->handler = h;
}

/** Changes the default error handler of the parser */

void OgdlParser_setErrorHandler (OgdlParser p, errorHandlerFunction h)
{
    p->errorHandler = h;
}

/** this is the default handler; it just prints the events
    that have content.
*/

void OgdlParser_printHandler(OgdlParser p, int lv, int type, char *s)
{
    int i;
    if (!type) return;
    
    for (i=0; i<lv*4; i++)
        putchar(' ');
    puts(s);
}

/** An event handler that creates a Graph nested structure holding
    the entire OGDL stream.
    
    XXX: errors are not reported to the parser!!! Errors in general
    to be reconsidered.
*/

void OgdlParser_graphHandler(OgdlParser p, int level, int type, char *s)
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
        p->g[0] = Graph_new("__root__");
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

static void event(OgdlParser p, int type, char *s)
{
    (*p->handler)(p,p->level,type,s);
}

/* Character classes:

   1 = WORD
   2 = SPACE
   3 = BREAK
   4 = END
   
*/

enum {
   C_WORD = 1,
   C_SPACE = 2,
   C_BREAK = 3,
   C_END = 4
};

static int _charType ( int c )
{
    if ( c == ' ' || c == '\t' )  return C_SPACE;
    if ( c == '\n' || c == '\r' ) return C_BREAK;
    if ( c < ' ' )                return C_END;
    if ( c >= 0xa0 )              return C_WORD;
    if ( c < 0x7f )               return C_WORD;
    return C_END;
}

/* Character input functions */

static int getChar(OgdlParser p)
{
    if (p->src_type) 
        return ((char*)p->src)[p->src_index++];
    else
        return p->last_char = getc((FILE*)p->src);
}

static void unGetChar(OgdlParser p)
{
    if (p->src_type)
        p->src_index--;
    else
        if (p->last_char != EOF)		/* XXX is this compatible with EOS ? */
            ungetc(p->last_char,(FILE*)p->src);
}

/* character classes */

static int isCharSpace(int c)
{
    return ((c == ' ') || (c == '\t')) ? 1 : 0;
}

static int isCharBreak(int c)
{
    return ((c == '\n') || (c == '\r')) ? 1 : 0;
}

/* A space() implementation that handles intermixed spaces and tabs. 

static int space(OgdlParser p, int check)
{
    int c, i;

    if (p->saved_space > 0) {
        i = p->saved_space;
        p->saved_space = 0;
        return i;
    }

    i = 0;
    while (1) {
        c = getChar(p);
        if (c == ' ') 
            i++;
        else if (c == '\t') 
            i = (i/p->tabs+1)*p->tabs;
        else
            break;
    }
    
    unGetChar(p);   
    return i;
}

*/

/* space() has control over p->tabs, and controls that tabs and spaces 
   are not mixed in indentation 

   Returns:
   
   0..n : number of spaces or tabs
   -9 : error   
*/

static int space(OgdlParser p, int check)
{
    int c, i, tabs=0, sps=0;

    if (p->saved_space > 0) {
        i = p->saved_space;
        p->saved_space = 0;
        return i;
    }

    i = 0;
    while (1) {
        c = getChar(p);
        if (c == ' ') {
            i++;
            sps++;
        }
        else if (c == '\t') {
            i++;
            tabs++;
        }
        else
            break;
    }
    
    if (check) {
        if ( (sps && tabs) || (p->tabs == ' ' && tabs) || (p->tabs == '\t' && sps)) 
            { error(p,ERROR_mixedTabsSpaces); return -9; }
        else if (i)
            p->tabs = sps?' ':'\t'; 
    }

    unGetChar(p);   
    return i;
}


/** newline : (CR LF) | CR | LF */

static int newline(OgdlParser p)
{
    int c = 0;

    if (p->saved_newline) {
        p->saved_newline = 0;
	p->is_comment = 0;
        return 1;
    }

    c = getChar(p);

    if (c == '\r') {
        c = getChar(p);
        if (c != '\n')
            unGetChar(p);
        p->line++;
	p->is_comment = 0;
        return 1;
    } else if (c == '\n') {
        p->line++;
	p->is_comment = 0;
        return 1;
    }

    unGetChar(p);
    return 0;
}

static int eos(OgdlParser p)
{
    int c;

    c = getChar(p);
    unGetChar(p);                   /* XXX check what happens with EOF */
    return ( _charType(c) == C_END ) ? 1 : 0;
}

/*
    Returns:
    
    0...n : number of characters
    -9 : error
*/

static int word(OgdlParser p)
{
    int c, t, i = 0, lv = C_SPACE;

    if ( p->is_comment ) lv = C_BREAK;

    while (1) {
        c = getChar(p);
        t = _charType(c);
	if ((i == 1) && (t == C_SPACE) && (p->buf[0]=='#')) 
        {
            lv = C_BREAK; 
            p->is_comment = 1;
        }
        if ( _charType(c) < lv ) {
            if (i>=BUFFER) { error(p,ERROR_textOverflow1); return -9; }
            p->buf[i++] = c;
        } else
            break;
    }

    if (i>=BUFFER) { error(p,ERROR_textOverflow2); return -9; }
    p->buf[i] = 0;
    unGetChar(p);
    
    /* If this was a comment, we don't actually return it */
    if (p->is_comment)
        return(0);

    return i;
}

/*
    Returns:
    
    0 : not a quoted string
    1 : is quoted string (now in p->buf)
    -9 : error
*/

static int quoted(OgdlParser p)
{
    int i=0, q, c, cc=0, flag = 0, n;

    n  = p->indentation[p->level];

    q = getChar(p);

    if (q != '"' && q != '\'') {
        unGetChar(p);
        return 0;
    }

    while (1) {
        c = getChar(p);
        if ((c == -1) || (c == q && cc != '\\'))
            break;

        if (flag) {
            if (isCharSpace(c)) {
                flag--;
                cc = c;
                continue;
            }
            else
                flag = 0;
        }
        
        if (i>=BUFFER) { error(p,ERROR_textOverflow3); return -9; }
        p->buf[i++] = c;
        cc = c;
        if (c == '\n')
            flag = n+1;
    }

    if (i>=BUFFER) { error(p,ERROR_textOverflow4); return -9; }
    p->buf[i] = 0;

    return 1;
}

/*
    Returns:
    
    0 : not a block
    1 : is a block (now in p->buf)
    -9 : error
*/

static int block(OgdlParser p, int n)
{
    int c, i = 0, j, m, ind = -1;

    c = getChar(p);

    if (c != '\\') {
        unGetChar(p);
        return 0;
    }

    if (!newline(p))
        return 0;   /* illegal or we need 2 ungetc's */

    /* all lines indented at least n are this block, and also less indented empty lines */

    for (;;) {
        m = space(p,1);
        if (m==-9) return -9;
        
        if (m < n) {
            if (newline(p)) {
                if (i>=BUFFER) { error(p,ERROR_textOverflow5); return -9; }
                p->buf[i++] = '\n';
            } else {
                p->saved_space = m;
                break;
            }
        } else {
            if (ind < 0)
                ind = m;                /* set ind to the indentation level of the first line */
            if (i>=BUFFER) { error(p,ERROR_textOverflow6); return -9; }
            for (j = ind; j < m; j++)     /* add those spaces that are not indentation */
                p->buf[i++] = ' ';
            while (1) {
                c = getChar(p);
                if (i>=BUFFER) { error(p,ERROR_textOverflow7); return -9; }
                if (c == EOF || c == '\n') {
                    p->buf[i++] = '\n';
                    break;
                }
                p->buf[i++] = c;
            }
            if (c == EOF)
                break;

        }
    }

    if (i>=BUFFER) { error(p,ERROR_textOverflow8); return -9; }
    p->buf[i] = 0;
    
    /* chomp (eliminate last break) */
    i--;
    if (i<0) return 1;
    if (isCharBreak(p->buf[i-1]))
        p->buf[i]=0;
    i--;
    if (i<0) return 1;
    if (isCharBreak(p->buf[i-1]))
        p->buf[i]=0;    
    
    return 1;
}

static int table (OgdlParser p, int n)
{
    int c, i = 0, m, lev,lev0;

    c = getChar(p);

    if (c != '|') {
        unGetChar(p);
        return 0;
    }

    if (!newline(p))
        return 0;   /* illegal or we need 2 ungetc's */

    lev0 = p->level;

    /* all lines indented at least n are this block, and also less indented empty lines */

    for (i=0;;) {
        m = space(p,1);
        if (m==-9) return -9;
        
        if (m < n) {
            if (newline(p)) 
                ;
            else if (eos(p)) 
                break;
            else {
                p->saved_space = m;
                p->level = p->line_level = lev0-1;             
                break;
            }
        } else {
            sprintf(p->buf,"%d",i);
            event(p,1, p->buf);
            lev=p->level++;
            p->line_level=lev+1;
            line(p);
            p->level=lev;
            i++;
        }
    } 

    return 1;
}


/* node ::= word | quoted | block 


   returns:
      0  : Not a node
      1  : More
      -1 :
      -9 : error
*/

static int node(OgdlParser p)
{
    int i=0,len,j;

    /* block indentation starts at current line indentation + 1, 
       not the indentation of the last node on the line. */
    
    j = block(p,p->indentation[p->line_level]+1); 
    if (j<0) return -9;
    
    if (j) {
        event(p,1,p->buf);
        return -1;
    }

    j = table(p,p->indentation[p->line_level]+1); 
    if (j<0) return -9;
    
    if (j) {
        event(p,1,p->buf);
        return -1;
    }
    
    j = quoted(p);
    if (j==-9) return -9;
    
    if (!j) {
        j = word(p);
        if (!j || j==-9) return j;
    }
    
    len = strlen(p->buf);
    if (!len) 
        strcpy(p->buf,"''");    /* avoid error */
    else 
        i = p->buf[len-1];
       
    if ( len == 1 && i == '(' ) {
        if (p->groupIndex>(GROUPS-1))
            { error(p,ERROR_maxGroups); return -9; }
        p->groups[p->groupIndex++] = p->level;
        event(p,0,"(");
        return 1;
    }
    else if (len == 1 && i == ')') {
        if (!p->groupIndex)
            { error(p,ERROR_negativeGroups); return -9; }
        
        p->level = p->groups[--(p->groupIndex)];
        
        event(p,0,")");
        return 1;
    }

    if ( i == ',' )
        p->buf[--len] = 0;
    
    if (len != 0)
        event(p,1, p->buf);

    if ( i == ',' ) {
        if (p->groupIndex>0) 
            p->level = p->groups[p->groupIndex-1];
        else 
            p->level = p->line_level;
    }
    else
        p->level++;   

    return 1;
}

/** line() : space? ( node ( space node )* )? space? newline

    returns:
      0 : EOS or error
      1 : more
*/

static int line(OgdlParser p)
{
    int i;
    
    i = space(p,1);
    if (i == -9) return 0;          /* error */
  
    if ( newline(p) ) return 1;     /* empty line */
    if ( eos (p) )    return 0;     /* end of stream */

    if ( p->level == 0 ) {
        p->indentation[0]=i;
        p->line_level=0;
    }
    else {
        if ( i > p->indentation[p->line_level] ) {
            p->line_level++;
            if (p->line_level>=LEVELS)
                { error(p,ERROR_maxLevels2); return 0; }
            p->indentation[p->line_level] = i;
        }
        else {
            if ( i < p->indentation[p->line_level] ) {
                while (p->line_level > 0) {
                    if ( i >= p->indentation[p->line_level] )
                        break;
                    p->line_level--;  
                }
            }
        }
    }
    
    p->level = p->line_level;

    while ( (i=node(p)) > 0) 
        if ( space(p,0) == -9 ) return 0;    /* error */
             
    if (i == -9 || eos(p))      /* error || end of stream */
        return 0;
    
    return 1;
}

/** Do the work */

int OgdlParser_parse (OgdlParser p, FILE *f)
{
    p->src = f;
    p->src_type = 0;	/* file */
    while ( line(p) );
    return 0;
}

int OgdlParser_parseString (OgdlParser p, char *s)
{
    p->src = s;
    p->src_type = 1;	/* string */
    p->src_index = 0;
    while ( line(p) );
    return 0;
}

Graph Ogdl_load (char * file)
{
    OgdlParser p;
    Graph g=0;
    FILE *f;
    
    f = fopen(file,"r");
    if (!f) return 0;

    p = OgdlParser_new();
    if (!p) {
        fclose(f);
	return 0;
    }
    
    OgdlParser_parse(p,f);

    fclose(f);
    
    /* free the parser but not the graph */
    if (p->g) {
        g = p->g[0];
        free (p->g);
    }
    free(p);    
    
    return g;
}
