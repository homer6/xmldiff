#include <string.h>
#include <stdlib.h>

#include "xml_parser.h"
#include "array.h"
#include "token.h"
#include "error.h"
#include "file.h"
#include "common_types.h"



static void free_token( void *token ){

    token_destroy( (struct token*)token );

}


void xml_parser_create( xml_parser *parser, size_t initial_token_array_size ){

    array_create( &parser->tokens, sizeof(token), initial_token_array_size, &free_token );

}



void xml_parser_destroy( xml_parser *parser ){

    array_destroy( &parser->tokens );

}



static void consume( xml_parser *parser, size_t number_of_elements ){

    parser->current_position += number_of_elements;

}


static void add_token( xml_parser *parser, int token_type, wide_char* contents, size_t contents_length ){

    token token;
    token_create( &token, token_type, contents, contents_length );

    array_add( &parser->tokens, &token );
    consume( parser, contents_length );

}


static int is_name_start_character( wide_char character ){

    //invalid
    if( character == ':' ) return TRUE;
    if( character == '_' ) return TRUE;
    if( character >= 'A' && character <= 'Z' ) return TRUE;
    if( character >= 'a' && character <= 'z' ) return TRUE;
    if( character >= 0xC0 && character <= 0xD6 ) return TRUE;
    if( character >= 0xD8 && character <= 0xF6 ) return TRUE;
    if( character >= 0xF8 && character <= 0x2FF ) return TRUE;
    if( character >= 0x370 && character <= 0x37D ) return TRUE;
    if( character >= 0x37F && character <= 0x1FFF ) return TRUE;
    if( character >= 0x200C && character <= 0x200D ) return TRUE;
    if( character >= 0x2070 && character <= 0x218F ) return TRUE;
    if( character >= 0x2C00 && character <= 0x2FEF ) return TRUE;
    if( character >= 0x3001 && character <= 0xD7FF ) return TRUE;
    if( character >= 0xF900 && character <= 0xFDCF ) return TRUE;
    if( character >= 0xFDF0 && character <= 0xFFFD ) return TRUE;
    if( character >= 0x10000 && character <= 0xEFFFF ) return TRUE;

    return FALSE;

}


static int is_name_char_character( wide_char character ){

    if( is_name_start_character(character) ) return TRUE;
    if( character == '-' ) return TRUE;
    if( character == '.' ) return TRUE;
    if( character == 0xB7 ) return TRUE;
    if( character >= '0' && character <= '9' ) return TRUE;
    if( character >= 0x0300 && character <= 0x036F ) return TRUE;
    if( character >= 0x203F && character <= 0x2040 ) return TRUE;

    return FALSE;

}


static int is_valid_character( wide_char character ){

    //invalid
    if( character >= 0x1 && character <= 0x9 ) return FALSE;
    if( character >= 0xB && character <= 0xC ) return FALSE;
    if( character >= 0xE && character <= 0xF ) return FALSE;
    if( character >= 0x7F && character <= 0x84 ) return FALSE;
    if( character >= 0x86 && character <= 0x9F ) return FALSE;
    if( character >= 0xFDD0 && character <= 0xFDDF ) return FALSE;
    if( character >= 0x1FFFE && character <= 0x1FFFF ) return FALSE;
    if( character >= 0x2FFFE && character <= 0x2FFFF ) return FALSE;
    if( character >= 0x3FFFE && character <= 0x3FFFF ) return FALSE;
    if( character >= 0x4FFFE && character <= 0x4FFFF ) return FALSE;
    if( character >= 0x5FFFE && character <= 0x5FFFF ) return FALSE;
    if( character >= 0x6FFFE && character <= 0x6FFFF ) return FALSE;
    if( character >= 0x7FFFE && character <= 0x7FFFF ) return FALSE;
    if( character >= 0x8FFFE && character <= 0x8FFFF ) return FALSE;
    if( character >= 0x9FFFE && character <= 0x9FFFF ) return FALSE;
    if( character >= 0xAFFFE && character <= 0xAFFFF ) return FALSE;
    if( character >= 0xBFFFE && character <= 0xBFFFF ) return FALSE;
    if( character >= 0xCFFFE && character <= 0xCFFFF ) return FALSE;
    if( character >= 0xDFFFE && character <= 0xDFFFF ) return FALSE;
    if( character >= 0xEFFFE && character <= 0xEFFFF ) return FALSE;
    if( character >= 0xFFFFE && character <= 0xFFFFF ) return FALSE;
    if( character >= 0x10FFFE && character <= 0x10FFFF ) return FALSE;

    //valid
    if( character >= 0x1 && character <= 0xD7FF ) return TRUE;
    if( character >= 0xE000 && character <= 0xFFFD ) return TRUE;
    if( character >= 0x10000 && character <= 0x10FFFF ) return TRUE;

    //else, invalid
    return FALSE;

}




static void match_whitespace( xml_parser *parser ){

    add_token( parser, TOKEN_TYPE_WHITESPACE, parser->current_position, 1 );

}


static void match_name( xml_parser *parser ){

    size_t x = 1;
    wide_char current_character;

    while(1){

        //detect end of file
        if( parser->current_position + x >= parser->end_of_file ){
            break;
        }

        current_character = *(parser->current_position + x);

        if( is_name_char_character(current_character) ){
            x++;
        }else{
            break;
        }

    }

    add_token( parser, TOKEN_TYPE_NAME, parser->current_position, x );

}





static void xml_parser_tokenize_from_file( xml_parser *parser, FILE *file ){

    //convert this to wide characters and convert to utf8 when outputting (and get the size in number of characters)
        wide_char *string;
        parser->file_contents_length = file_get_contents( &string, file );


    //skip over the BOM
        unsigned int *int_iterator = (unsigned int *)string;
        if( *int_iterator == 65279 ){
            parser->current_position = string + 1;
            parser->file_contents_length--;
        }else{
            parser->current_position = string;
        }
        parser->end_of_file = parser->current_position + parser->file_contents_length;


    //predict each token
        wide_char current_character;
        while( parser->current_position < parser->end_of_file ){

            current_character = *parser->current_position;

            switch( current_character ){

                case ' ':
                case '\n':
                case '\r':
                case '\t':
                    //match_whitespace( parser );
                    consume( parser, 1 );
                    continue;


            };


            if( is_name_start_character(current_character) ){
                match_name( parser );
                continue;
            }

            consume( parser, 1 );

        }


    free( string );

}



void xml_parser_parse_from_file( xml_element *xml_element, xml_parser *parser, FILE *file ){

    xml_element_create( xml_element, "element_name" );

    xml_parser_tokenize_from_file( parser, file );

    printf( "There are %lu tokens.\n", array_size(&parser->tokens) );

    token current_token;
    while( array_pop(&parser->tokens, &current_token) ){

        /*
        switch( current_token.type ){

            case TOKEN_TYPE_WHITESPACE:
                printf( "Token Whitespace\n" );
                break;

        };*/

        token_print( &current_token );
        token_destroy( &current_token );

    }


}



