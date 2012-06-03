#ifndef XMLDIFF_XML_ELEMENT_H
#define XMLDIFF_XML_ELEMENT_H



typedef struct xml_element{
    char *tag_name;
}xml_element;


void xml_element_create( xml_element *xml_element, const char *tag_name );
void xml_element_destroy( xml_element *xml_element );



#endif
