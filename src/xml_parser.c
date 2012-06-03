#include <string.h>
#include <stdlib.h>

#include "xml_parser.h"
#include "array.h"
#include "token.h"
#include "error.h"


static void free_token( void *token ){

    token_destroy( (struct token*)token );

}


void xml_parser_create( xml_parser *parser, size_t initial_token_array_size ){

    array_create( &parser->tokens, sizeof(token), initial_token_array_size, &free_token );

}



void xml_parser_destroy( xml_parser *parser ){

    array_destroy( &parser->tokens );

}




static void xml_parser_tokenize_from_file( xml_parser *parser, FILE *file ){

    //convert this to wide characters and convert to utf8 when outputting

    char **string = malloc( sizeof(char*) );
    file_get_contents( string, file );


    size_t file_contents_length = strlen( *string );
    size_t x = 0;


    while( x < file_contents_length ){


        x++;

    }



    free( *string );
    free( string );

}


void xml_parser_parse_from_file( xml_element *xml_element, xml_parser *parser, FILE *file ){

    xml_element_create( xml_element, "element_name" );

    xml_parser_tokenize_from_file( parser, file );

    token *current_token;
    while( array_pop(&parser->tokens, current_token) ){

        break;

    }

}



