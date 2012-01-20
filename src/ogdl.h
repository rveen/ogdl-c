/** \file ogdl.h */

#ifndef	_OGDL_H
#define _OGDL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VERSION "20120120"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32) && (!defined(__BORLANDC__) || (__BORLANDC__ >= 0x500)) && \
    defined(_DLL)
#define EXTERN extern _declspec(dllexport)
#else
#define EXTERN extern 
#endif

/** byte reader */

typedef int (*readFunction)(int fd);

/** event handler (pointer to object, level, type, byte string) */
 
#define EVENT_FORMAT  0
#define EVENT_TEXT    1
#define EVENT_BINARY  2
#define EVENT_COMMENT 3

typedef void (*eventHandlerFunction)(void *object, int level, int type, char *data);

/** error handler (pointer to object, error number) */

typedef void (*errorHandlerFunction)(void *object, int errno);

// XXX should have negative values:

enum { 
    ERROR_none,
    ERROR_malloc,
    ERROR_maxLevels,
    ERROR_negativeLevels,
    ERROR_nullGraph,
    ERROR_mixedTabsSpaces,
    ERROR_textOverflow1,
    ERROR_textOverflow2,
    ERROR_textOverflow3,
    ERROR_textOverflow4,
    ERROR_textOverflow5,
    ERROR_textOverflow6,
    ERROR_textOverflow7,
    ERROR_textOverflow8,
    ERROR_maxGroups,
    ERROR_negativeGroups,
    ERROR_maxLevels2,
    ERROR_notFound,
    ERROR_realloc,
    ERROR_argumentOutOfRange,
    ERROR_noObject,
    ERROR_argumentIsNull,
    ERROR_max /* Not actually a valid error number */
};


/** Graph */

typedef struct _Graph {
    char * name;
    char * type;
    int    size;
    int    size_max;
    struct _Graph **nodes;
} * Graph;

EXTERN Graph   Graph_new             (char * name);
EXTERN void    Graph_free            (Graph g);
EXTERN Graph   Graph_get             (Graph g, char * path);
EXTERN char *  Graph_getString       (Graph g, char * path);
EXTERN int     Graph_set             (Graph g, char * path, Graph node);
EXTERN Graph   Graph_md              (Graph g, char * path);
EXTERN Graph   Graph_getNode         (Graph g, char * name);
EXTERN int     Graph_setName         (Graph g, char *s);
EXTERN int     Graph_addNode         (Graph g, Graph node);
EXTERN Graph   Graph_add             (Graph g, char *name);
EXTERN void    Graph_print           (Graph g);
EXTERN void    Graph_fprint          (Graph g, FILE *fp, int maxlevel, int nspaces, int mode);
EXTERN int     Graph_printString     (const char *s, int indent, int pending_break);
EXTERN int     Graph_fprintString    (FILE *fp, const char *s, int indent, int pending_break);
EXTERN int     Graph_size            (Graph g);
EXTERN Graph   Graph_getByIndex      (Graph g, int index);
EXTERN char *  Graph_getNameByIndex  (Graph g, int index);
EXTERN char *  Graph_getName         (Graph g);


/** Path */

EXTERN char *  Path_element       (char * path, char * buf);

#define LEVELS 128
#define GROUPS 128
#define BUFFER 65534    /* lower that int16 maxvalue, just in case */
#define OGDL_EOS '\f'   /* XXX any char < 0x20 except NL, CR, TAB */

/** OgdlParser */

typedef struct _OgdlParser {
    char buf[BUFFER];   /* XXX dynamic allocation */
    int level;

    int last_char;
    int groups[GROUPS];
    int groupIndex;
    int line;
    int line_level;
    int saved_space;
    int saved_newline;
    int tabs;           /* number of spaces per tab */
    int indentation[LEVELS];
    
    eventHandlerFunction handler;   
    errorHandlerFunction errorHandler; 

    void *src;
    int  src_index;
    int  src_type;
    
    Graph *g;
    int is_comment;
} * OgdlParser;

EXTERN OgdlParser   OgdlParser_new              (void);
EXTERN OgdlParser   OgdlParser_reuse            (OgdlParser p);
EXTERN OgdlParser   OgdlParser_detachGraph      (OgdlParser p);
EXTERN void         OgdlParser_free             (OgdlParser p);
EXTERN void         OgdlParser_setHandler       (OgdlParser p, eventHandlerFunction ev);
EXTERN int          OgdlParser_parse            (OgdlParser p, FILE * f);
EXTERN int          OgdlParser_parseString      (OgdlParser p, char * s);
EXTERN void         OgdlParser_graphHandler     (OgdlParser p, int level, int type, char * s);
EXTERN void         OgdlParser_printHandler     (OgdlParser p, int level, int type, char * s);
EXTERN Graph        Ogdl_load                   (char * fileName);
EXTERN void         OgdlParser_error            (OgdlParser p, int n);
EXTERN void         OgdlParser_setErrorHandler  (OgdlParser p, errorHandlerFunction h);
EXTERN const char * OgdlParser_getErrorMessage  (int n);

#define OGDL_ERROR_TABS_SPACES    5

/** OgdlBinParser */

typedef struct _OgdlBinParser 
{
    char buf[BUFFER];   /* XXX dynamic allocation */
    int level;

    eventHandlerFunction handler;   
    errorHandlerFunction errorHandler; 
    
    int  len;

    readFunction         read;
    int                  readfd; /* file descriptor to read from */    
    Graph *g;
    
} * OgdlBinParser;

EXTERN OgdlBinParser   OgdlBinParser_new          (readFunction readf, int fd);
EXTERN void            OgdlBinParser_free         (OgdlBinParser p);
EXTERN Graph           OgdlBinParser_parse        (OgdlBinParser p);
EXTERN void            OgdlBinParser_graphHandler (OgdlBinParser p, int level, int type, char *s);

/** OgdlLog */

typedef struct _OgdlLog {
    FILE * f;
    OgdlParser p;
} * OgdlLog;

EXTERN OgdlLog        OgdlLog_new      (char *fileName);
EXTERN void           OgdlLog_free     (OgdlLog l);
EXTERN unsigned long  OgdlLog_add      (OgdlLog l, Graph g);
EXTERN Graph          OgdlLog_get      (OgdlLog l, unsigned long offset);
EXTERN Graph          OgdlLog_next     (OgdlLog l);
EXTERN unsigned long  OgdlLog_position (OgdlLog l);

#ifdef __cplusplus
}
#endif

#endif
