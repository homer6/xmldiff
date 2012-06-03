#ifndef XMLDIFF_TOKEN_H
#define XMLDIFF_TOKEN_H


enum token_type{
   TOKEN_TYPE_XML_DECLARATION,
   TOKEN_TYPE_TAG_OPEN,
   TOKEN_TYPE_TAG_CLOSE,
   TOKEN_TYPE_TAG_SELF_CLOSE,
   TOKEN_TYPE_TAG_NAME,
   TOKEN_TYPE_ATTRIBUTE_NAME,
   TOKEN_TYPE_ATTRIBUTE_EQUALS,
   TOKEN_TYPE_ATTRIBUTE_VALUE,
   TOKEN_TYPE_WHITESPACE
};


typedef struct token{
    int type;
    char *contents;
}token;


void token_create( token *token, int type, const char *contents );
void token_destroy( token *token );



#endif
