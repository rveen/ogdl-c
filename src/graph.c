/** \file graph.c
*/

#include "ogdl.h"

#define CHUNK 16
#define MAXSTRING 65534

static void fatal(char *s)
{
    printf("graph.c: fatal: %s\n",s);
    exit(1);
}

static void error(char *s)
{
    fprintf(stderr,"graph.c: error: %s\n",s);
}

/** Graph constructor. The name argument can be any string.
    
    XXX Don't like the error handling.
 */

Graph Graph_new (char *name)
{
    Graph g;
    int i;

    if (!name) {
        error("argument is null");
	return 0;
    }
    
    g = (void *) malloc (sizeof(*g));
    if (!g) {
        error("malloc error");
        return 0;
    }
    
    g->size = 0;
    g->type = 0;
    g->nodes = 0;

    /* limit string lengths */
    i = strlen(name);
    if (i>MAXSTRING || !i) {
        free (g);
	error("string too long or empty");
        return 0;
    }
    
    if (name) {
        g->name = malloc(i+1);
        if (!g->name) {
	    error("malloc error");
	    free (g);
	    return 0;
	}
        strncpy(g->name,name,i);
	g->name[i]=0;
    }

    return g;
}

/** Return the number of subnodes */

int Graph_size(Graph g)
{
    return g->size;
}

/** Return a subnode by index */

Graph Graph_getByIndex(Graph g, int index)
{
    if (index >= g->size)
        return NULL;
    return g->nodes[index];
}

/** Return a subnode's name by index */

char * Graph_getNameByIndex(Graph g, int index)
{
    if (index >= g->size)
        return NULL;
    return g->nodes[index]->name;
}

/** Return the name of a Graph */

char * Graph_getName(Graph g)
{
    if (g == NULL)
        return NULL;
    return g->name;
}

/** set the name of this node.

    A newly allocated copy of the string is
    used. Return values are non zero on error.
 */

int Graph_setName (Graph g, char *s)
{
    int len;
    char *p;
        
    if (!g)
        return ERROR_noObject;

    if (!s)
        return ERROR_argumentIsNull;
	
    len = strlen(s);
    if (len>MAXSTRING) 
        return ERROR_argumentOutOfRange;
	
    p = malloc(len+1);
    if (!p) 
	return ERROR_malloc;

    strncpy(p,s,len);
    p[len]=0;

    if (g->name) 
        free(g->name);

    g->name = p;
    return 0;
}

/** Graph destructor */

void Graph_free (Graph g)
{
    int i;
    
    if (!g) return;

    if (g->nodes) {
        for (i=0; i<g->size; i++)
            Graph_free(g->nodes[i]);
        free(g->nodes);
    }
    
    if (g->name)
        free(g->name);

    free (g);
}

/** Returns zero when the line has been 'closed', ie, a NL has been printed.
 *
 * BUG(): This function should print lines with spaces but without nl's as quoted,
 * because they may not be terminal nodes. Lines with nl's can be considered leaf
 * or terminal nodes (in practice).
 *
 * BUG(): This function should handle comments correctly.
*/

int Graph_fprintString (FILE *fp, const char *s, int indent, int pending_break)
{
        int i, j, sp=0, nl=0, len;

        if (!s) return 0;
        
        len = strlen(s);

        /* analize string for spaces and newlines */
        
        for (i=0; i<len; i++) {
            if (s[i] == ' ' || s[i] == '\t')
                sp=1;
            else if (s[i] == '\n' || s[i] == '\r')
                nl=1;

            if (nl && sp)
                break;
        }

		if (nl || sp) {

			if (indent>0) {
			    fputs(" \\\n", fp);
			}

			for (j = 0; j < indent; j++)
				fputc(' ', fp);

			for (i = 0; i < len; i++) {
				fputc(s[i], fp);
				if (i<(len-1) && s[i] == '\n')
					for (j = 0; j < indent; j++)
						fputc(' ', fp);

			}
			if (s[i-1]!='\n')
				fputc('\n',fp);

			return 0;
		}
        
        /* printing simple strings */
		if (pending_break)
		    fputc('\n',fp);
        
        for (i=0; i<indent; i++)
            fputc(' ',fp);
            
	    if (s[0] == '#')
	        sp = 0;
        
        for (i=0; i<len; i++)
            fputc(s[i],fp);
            
        return 1;
}

int Graph_printString(const char *s, int indent, int pending_break)
{
    return Graph_fprintString(stdout,s,indent,pending_break);
}

static int _fprintGraph(FILE *fp, Graph g, int level, int maxLevel, int nspaces, int pending_break)
{
    int i, j, n;

    if ((maxLevel != -1) && (level >= maxLevel)) return pending_break;

    if (!g) return 0;

    n = level * nspaces;

    j = Graph_fprintString(fp,g->name,n,pending_break);

    for (i=0; i<g->size; i++)  
        j = _fprintGraph(fp,g->nodes[i],level+1,maxLevel,nspaces,j);
   
    return j;
}

/** Prints a Graph in OGDL format. 
 */

void Graph_print (Graph g)
{
    int i,j=0;
    
    if (!g) return;
    
    for (i=0; i<g->size; i++)
        j = _fprintGraph(stdout,g->nodes[i],0,-1,4,j);
    
    if (j)
        fputc('\n',stdout);
}

void Graph_fprint (Graph g, FILE *fp, int max, int nspaces, int mode)
{
    int i, j=0;
    
    if (!g) return;

    if (mode)
        for (i=0; i<g->size; i++)
            j = _fprintGraph(fp,g->nodes[i],0,max,nspaces,j);
    else 
        j = _fprintGraph(fp,g,0,max,nspaces,j);
    
    if (j)
        fputc('\n',fp);
}

/** add a node to a graph */

int Graph_addNode(Graph g, Graph node)
{
    if (!g)
        return ERROR_noObject;
    
    if (!node) 
        return ERROR_argumentIsNull;
    
    if (!g->nodes) {
        g->nodes = (void *) malloc( CHUNK * sizeof(g) );
        if (!g->nodes) 
            return ERROR_malloc;
        g->size_max = CHUNK;
    }
    
    if (g->size>=g->size_max) {
        Graph *p;
        
        p = realloc(g->nodes, (g->size_max+CHUNK) * sizeof(g) );    

        if (!p) 
            return ERROR_realloc;
        
        g->nodes = p;
        g->size_max += CHUNK;
    }
    
    if (g->size >= g->size_max)
        fatal("addNode: programmer error");
        
    g->nodes[g->size++] = node;

    return 0;
}

/** Add a node to a graph, from a string */

Graph Graph_add (Graph g, char *name)
{
	Graph node = Graph_new(name);
	Graph_addNode(g,node);
	return node;
}

/** Return a subnode by index. */

Graph Graph_getNode (Graph g, char * name)
{
    int i;
    Graph node;
    
    if (!name || !g) return 0;

    for (i=0; i<g->size; i++) {
        node = g->nodes[i];
        if (!strncmp(name,node->name,MAXSTRING))
	   return node;
    }
    return 0;
}

/** Returns a Graph pointer that matches the given path, or
    null in case the path doesn't resolve. */

Graph Graph_get (Graph g, char * path)
{
    char e[256], last[256], *p;
    Graph up=0, node;
    int i,j,n;

    p = path;
    last[0] = 0;    /* used also to distinguish between x.[n] and x[n] */
    
    while ( (p=Path_element(p,e)) )
    {
        if (e[0] == '[') {

            n = atoi(e+1);  

            if (n < 0)     
                return NULL;        /* is a syntax error, but ... */    
            if (e[1]) {
                if (g->size > n)
                    g = g->nodes[n];
                else 
                    return NULL;
            }
            else {
                if (up == NULL) return NULL;
                
                if (e[1] == 0) {        /* this means [] */
                    /* new graph and get all elements with this name */
                    g = Graph_new("__vector__");

                    for (i=0; i<up->size; i++) {
                        node = up->nodes[i];
                        if (!strncmp(node->name,last,256))
                            for (j=0; j<node->size; j++)
                                Graph_addNode(g,node->nodes[j]);
                    }
                }
                else if (n>=0) {

                    for (i=0; i<up->size; i++) {
                        node = up->nodes[i];
                  
			if (!strncmp(node->name,last,256)) {
	
                            if (!n--) {
                                g = node;
				
                                break;
                            }
                        }
                    }  
		    if (n>-1) return NULL;              
                }
                last[0] = 0;
            }
        }
        else if (e[0] == '.')
            last[0] =  0;
        else {
            up = g;
            if (! (g=Graph_getNode(g,e)))
                return 0;
            strncpy(last,e,255);
        }
    }
    return g;
}

char * Graph_getString(Graph g, char * path)
{
    Graph node;
    
    node = Graph_get(g,path);
    if (!node || !node->nodes || !node->nodes[0])
        return 0;
    return node->nodes[0]->name;
}

int Graph_set (Graph g, char * path, Graph v)
{
    Graph node;
    
    if (!g || !path || !path[0]) return 1;
    
    node = Graph_get(g,path);

    if (node) {
        if (!node->size)
	    Graph_addNode(node,v);
	else
	    node->nodes[0] = v;    /* XXX memory leak ! */
	return 0;
    }
    
    node = Graph_md(g,path);        
    Graph_addNode(node,v);
    Graph_addNode(g,node);
    return 0;
}

/** Creates a path in a graph. Like making a directory */

Graph Graph_md (Graph g, char * path)
{
    char e[256], last[256], *p;
    Graph up=0, node;
    int i,n;
    
    if (!g || !path || !path[0]) return 0;
    
    p = path;
    last[0] = 0;    /* used also to distinguish between x.[n] and x[n] */
    
    while ( (p=Path_element(p,e)) )
    {
        if (e[0] == '[') {

            n = atoi(e+1);  

            if (!last[0]) {
                if (n < 0 || !e[1])     /* allow [0], but not [] */
                    return 0;           /* is a syntax error, but ... */    
                if (g->size > n)
                    g = g->nodes[n];
                else 
		    return 0;		/* not allowed: cannot create unnamed nodes */
            }
            else {
                if (!up) return 0;
                
                if (e[1] == 0)          /* this means [] */
                    return 0;		/* not allowed */
                
                else if (n>=0) {
                    for (i=0; i<up->size; i++) {
                        node = up->nodes[i];
                        if (!strncmp(node->name,last,256)) {
                            if (!n--) {
                                g = node;
                                break;
                            }
                        }
                    }                
                }
                last[0] = 0;
            }
        }
        else if (e[0] == '.')
            last[0] =  0;
        else {
            up = g;
            if (! (g=Graph_getNode(g,e))) {
                node = Graph_new(e);
		Graph_addNode(g,node);
		g = node;
	    }
            strncpy(last,e,255);
        }
    }
    return g;

}
