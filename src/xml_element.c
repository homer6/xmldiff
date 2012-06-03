#include <string.h>
#include <stdlib.h>

#include "xml_element.h"
#include "error.h"



void xml_element_create( xml_element *xml_element, const char *tag_name ){

    size_t tag_name_length = strlen( tag_name );

    xml_element->tag_name = malloc( tag_name_length + 1 );
    if( xml_element->tag_name == NULL ){
        error_fatal( "Could not allocate memory for xml element tag name." );
    }
    strcpy( xml_element->tag_name, tag_name );

}


void xml_element_destroy( xml_element *xml_element ){

    free( xml_element->tag_name );

}



