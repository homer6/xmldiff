#ifndef XMLDIFF_TOKEN_H
#define XMLDIFF_TOKEN_H


enum token_type{
   XML_DECLARATION,
   TAG_OPEN,
   TAG_CLOSE,
   TAG_SELF_CLOSE,
   TAG_NAME,
   ATTRIBUTE_NAME,
   ATTRIBUTE_EQUALS,
   ATTRIBUTE_VALUE,
   WHITESPACE
};


struct{
    int type;
    char *contents;
}token;





#endif
