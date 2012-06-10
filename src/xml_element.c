#include <string.h>
#include <stdlib.h>

#include "xml_element.h"
#include "error.h"


void xml_element_create( xml_element *xml_element, wide_char *tag_name, unsigned char tag_name_length ){

    xml_element->tag_name = tag_name;
    xml_element->tag_name_length = tag_name_length;

}


void xml_element_destroy( xml_element *xml_element ){

    //just to avoid compiler warning
    xml_element = xml_element;
    //free( xml_element->tag_name );

}



