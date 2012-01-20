/** \file path.c

    Parses a string into path elements.

    Grammar:

        path :  e1(.e)*
        e : s arglist? | [path?] | {path?}
        e1 : s arglist?
        s : char_word+ | single_quoted | double_quoted
        arglist : '(' (path space? ',' space)* path? ')'
        char_word : any printable char less separators []{};., and space
        
        
    PENDING:    
    
    - escape sequences \' \"
*/

#include "ogdl.h"

int isWordChar(char);

/** copy the next element of a path to buf */

char * Path_element (char * path, char * buf)
{
    char c;

    if (!*path)
        return 0;

    switch (c=*path)
    {
        case '.':
            *buf++ = *path++;
            break;

        case '[' :

            while ((c=*path) && c != ']' )
                *buf++ = *path++;
            path++;
            break;

        case '{':

            while ((c=*path) && c != '}' )
                *buf++ = *path++;
            path++;
            break;
            
        case '\'':
            while ((c=*path) && c != '\'' )
                *buf++ = *path++;
            path++;
            break;
        
        case '"':
            while ((c=*path) && c != '"' )
                *buf++ = *path++;
            path++;
            break;
        
        default:

            while ((c=*path) && isWordChar(c) )
                *buf++ = *path++;

    }
    
    *buf = 0;
        
    return path;
}

int isWordChar ( char c )
{
     switch (c) {
         case '.':
         case '[':
         case '(':
         case '{':
         case ']':
         case ')':
         case '}':         
         case '"':
         case ';':
         case ',':
         case ' ':
         case '\t':
         case 0:
             return 0;
         default:
             return 1;
    }
}

