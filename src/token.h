#ifndef XMLDIFF_TOKEN_H
#define XMLDIFF_TOKEN_H


#include "file.h"  //for wide_char

//see: http://www.w3.org/TR/xml11/

enum token_type{   
   TOKEN_TYPE_INVALID,
   TOKEN_TYPE_NAME,
   TOKEN_TYPE_WHITESPACE,
   TOKEN_TYPE_UNKNOWN
};


typedef struct token{
    int type;
    wide_char *contents;
    size_t contents_length;     //in characters (4 bytes per character)
}token;


void token_create( token *token, int type, const wide_char *contents, size_t contents_length );
void token_destroy( token *token );


void token_print( token *token );



#endif
