#ifndef XMLDIFF_XML_ELEMENT_H
#define XMLDIFF_XML_ELEMENT_H

#include "file.h"
#include "tree.h"


typedef struct xml_element{
    wide_char *tag_name;
    unsigned char tag_name_length;
    tree_node *tree_node;
}xml_element;


void xml_element_create( xml_element *xml_element, wide_char *tag_name, unsigned char tag_name_length );
void xml_element_destroy( xml_element *xml_element );



#endif
