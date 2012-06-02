#ifndef XMLDIFF_TOKEN_H
#define XMLDIFF_TOKEN_H


enum token_type{
   TOKEN_XML_DECLARATION,
   TOKEN_TAG_OPEN,
   TOKEN_TAG_CLOSE,
   TOKEN_TAG_SELF_CLOSE,
   TOKEN_TAG_NAME,
   TOKEN_ATTRIBUTE_NAME,
   TOKEN_ATTRIBUTE_EQUALS,
   TOKEN_ATTRIBUTE_VALUE,
   TOKEN_WHITESPACE
};


typedef struct token{
    int type;
    char *contents;
}token;


token *token_create( int type, const char *contents );
void token_destroy( token *token );



#endif
