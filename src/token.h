#ifndef XMLDIFF_TOKEN_H
#define XMLDIFF_TOKEN_H


#include "file.h"  //for wide_char

//see: http://www.w3.org/TR/xml11/

enum token_type{   
   TOKEN_TYPE_INVALID,
   TOKEN_TYPE_NAME,
   TOKEN_TYPE_WHITESPACE,
   TOKEN_TYPE_LESS_THAN,     //  <
   TOKEN_TYPE_GREATER_THAN,  //  >
   TOKEN_TYPE_END_TAG_OPEN,  //  </
   TOKEN_TYPE_UNKNOWN
};


typedef struct token{
    int type;
    wide_char *start_position;
    wide_char *end_position;
}token;


void token_create( token *token, int type, wide_char *start_position, size_t contents_length );
void token_destroy( token *token );


void token_print( token *token );



#endif
