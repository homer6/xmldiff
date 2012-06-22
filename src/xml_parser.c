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

static void xml_element_destory_void( void *element_location ){

    xml_element *element = *(xml_element**)element_location;

    xml_element_destroy( element );

}



void xml_parser_create( xml_parser *parser, size_t initial_token_array_size ){

    array_create( &parser->tokens, sizeof(token), initial_token_array_size, &free_token );
    parser->ast_root_node = malloc( sizeof(tree_node) );
    parser->current_ast_node = NULL;

}



void xml_parser_destroy( xml_parser *parser ){

    array_destroy( &parser->tokens );
    free( parser->file_contents );
    tree_node_destroy( parser->ast_root_node );

}


static int lookahead_while( xml_parser *parser, int(*comparison_function)(wide_char), int *number_of_characters_found );


/**
 * Determines if the offset character from the current position matches the
 * given character. Returns FALSE if the address is beyond the bounds of the
 * file.
 *
 * @return bool
 */
static int match_character( xml_parser *parser, int offset, const wide_char character ){

    wide_char *lookaround = parser->current_position + offset;

    if( lookaround >= parser->end_of_file || lookaround < parser->file_contents ){
        return -1;
    }

    if( character == *lookaround ){
        return 1;
    }

    return 0;

}


/**
 * Advances the current position by number_of_elements.
 *
 */
static void consume_character( xml_parser *parser, size_t number_of_elements ){

    parser->current_position += number_of_elements;

}

/**
 * Advances the current position by number_of_elements.
 *
 */
static void consume_token( xml_parser *parser, size_t number_of_elements ){

    parser->current_token_index += number_of_elements;

}


/**
 * Adds a token of contents_length length and consume_characters those characters.
 *
 */
static void add_token( xml_parser *parser, int token_type, wide_char* start_position, size_t contents_length ){

    token token;
    token_create( &token, token_type, start_position, contents_length );

    array_add( &parser->tokens, &token );
    consume_character( parser, contents_length );

}


static int is_name_start_character( wide_char character ){


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

    if( character >= 0xFDD0 && character <= 0x10FFFF ){
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
    }

    //valid
    if( character >= 0x1 && character <= 0xD7FF ) return TRUE;
    if( character >= 0xE000 && character <= 0xFFFD ) return TRUE;
    if( character >= 0x10000 && character <= 0x10FFFF ) return TRUE;

    //else, invalid
    return FALSE;

}


static int is_whitespace( wide_char character ){

    if( character == ' ' || character == '\t' || character == '\n' || character == '\r' ) return TRUE;
    return FALSE;

}


static int match_whitespace( xml_parser *parser ){

    int number_of_characters;

    lookahead_while( parser, &is_whitespace, &number_of_characters );

    if( number_of_characters > 0 ){
        add_token( parser, TOKEN_TYPE_WHITESPACE, parser->current_position, number_of_characters );
        return TRUE;
    }

    return FALSE;

}






static int match_end_tag_open( xml_parser *parser ){

    if(
        match_character( parser, 0, '<' ) &&
        match_character( parser, 1, '/' )
    ){
        add_token( parser, TOKEN_TYPE_END_TAG_OPEN, parser->current_position, 2 );
        return TRUE;
    }
    return FALSE;

}




static int match_less_than( xml_parser *parser ){

    if( match_character(parser, 0, '<') ){
        add_token( parser, TOKEN_TYPE_LESS_THAN, parser->current_position, 1 );
        return TRUE;
    }
    return FALSE;

}


static int match_greater_than( xml_parser *parser ){

    if( match_character(parser, 0, '>') ){
        add_token( parser, TOKEN_TYPE_GREATER_THAN, parser->current_position, 1 );
        return TRUE;
    }
    return FALSE;

}


static int match_name( xml_parser *parser ){

    int number_of_characters;

    lookahead_while( parser, &is_name_char_character, &number_of_characters );

    if( number_of_characters > 0 ){
        add_token( parser, TOKEN_TYPE_NAME, parser->current_position, number_of_characters );
        return TRUE;
    }
    return FALSE;

}



/**
 *
 *
 */
static int lookahead_while( xml_parser *parser, int(*comparison_function)(wide_char), int *number_of_characters_found ){

    size_t x = 0;
    wide_char current_character;
    unsigned char end_of_file_hit = 0;

    while(1){

        //detect end of file
        if( parser->current_position + x >= parser->end_of_file ){
            end_of_file_hit = 1;
            break;
        }

        current_character = *(parser->current_position + x);

        if( comparison_function(current_character) ){
            x++;
        }else{
            break;
        }

    }

    *number_of_characters_found = x;

    if( end_of_file_hit ){
        return 1; //error code #1
    }else{
        return 0; //no error
    }

}


/**
 * Determines if the offset token from the current position matches the
 * given token type. Returns -1 if the address is beyond the bounds.
 * Returns 1 if the types match. Returns 0 if there's no match.
 *
 */
static int match_token_type( xml_parser *parser, int offset, int type ){

    size_t lookaround_index = parser->current_token_index + offset;

    token *lookaround_token = array_get_element_at( &parser->tokens, lookaround_index );

    if( lookaround_token == NULL ){
        return -1;
    }

    if( lookaround_token->type == type ){
        return 1;
    }

    return 0;

}


/**
  * Gets the token at the offset from the current_token_index.
  * Returns NULL if not found.
  *
  */
static token *get_token_at( xml_parser *parser, int offset ){

    size_t lookaround_index = parser->current_token_index + offset;

    return array_get_element_at( &parser->tokens, lookaround_index );

}





static int parse_element_start_tag( xml_parser *parser ){

    if(
        match_token_type(parser, 0, TOKEN_TYPE_LESS_THAN) &&
        match_token_type(parser, 1, TOKEN_TYPE_NAME) &&
        match_token_type(parser, 2, TOKEN_TYPE_GREATER_THAN)
    ){
        token *current_token = get_token_at( parser, 1 );

        xml_element *element = malloc( sizeof(xml_element) );
        xml_element_create( element, current_token->start_position, (unsigned char)(current_token->end_position - current_token->start_position) );

        if( parser->current_ast_node == NULL ){
            //this is the root element
            tree_node_create( parser->ast_root_node, element, &xml_element_destory_void );
            parser->current_ast_node = parser->ast_root_node;
            element->tree_node = parser->ast_root_node;
            //printf( "Matched root\n");
        }else{
            parser->current_ast_node = tree_node_add_last_child( parser->current_ast_node, element );
            element->tree_node = parser->current_ast_node;
            //printf( "Matched new element\n");
        }

        //token_print( current_token );

        consume_token( parser, 3 );

        return TRUE;
    }

    return FALSE;

}


static int parse_element_end_tag( xml_parser *parser ){

    if(
        match_token_type(parser, 0, TOKEN_TYPE_END_TAG_OPEN) &&
        match_token_type(parser, 1, TOKEN_TYPE_NAME) &&
        match_token_type(parser, 2, TOKEN_TYPE_GREATER_THAN)
    ){

        if( parser->current_ast_node == NULL ){
            error_fatal( "Malformed XML. Too many close tags." );
        }else{
            parser->current_ast_node = parser->current_ast_node->parent;
        }
        consume_token( parser, 3 );

        return TRUE;

    }


    //or with an optional whitespace
    if(
        match_token_type(parser, 0, TOKEN_TYPE_END_TAG_OPEN) &&
        match_token_type(parser, 1, TOKEN_TYPE_NAME) &&
        match_token_type(parser, 2, TOKEN_TYPE_WHITESPACE) &&
        match_token_type(parser, 3, TOKEN_TYPE_GREATER_THAN)
    ){

        if( parser->current_ast_node == NULL ){
            error_fatal( "Malformed XML. Too many close tags." );
        }else{
            parser->current_ast_node = parser->current_ast_node->parent;
        }
        consume_token( parser, 4 );

        return TRUE;

    }


    return FALSE;

}





static void xml_parser_tokenize_from_file( xml_parser *parser, FILE *file ){


    //convert this to wide characters (and get the size in number of characters)
    //wide charactes are 4 bytes each
    //make sure to convert to utf8 when outputting (later)
        parser->file_contents_length = file_get_contents( &parser->file_contents, file );


    //skip over the unicode BOM: http://en.wikipedia.org/wiki/Byte_order_mark
        unsigned int *int_iterator = (unsigned int *)parser->file_contents;
        if( *int_iterator == 65279 ){
            parser->current_position = parser->file_contents + 1;
            parser->file_contents_length--;
        }else{
            parser->current_position = parser->file_contents;
        }
        parser->end_of_file = parser->current_position + parser->file_contents_length;


    //Tokenizing: predict and match each token from the character stream
        wide_char current_character;
        while( parser->current_position < parser->end_of_file ){

            current_character = *parser->current_position;

            switch( current_character ){

                case ' ':
                case '\n':
                case '\r':
                case '\t':
                    if( match_whitespace(parser) ) continue;
                    break;

                case '<':
                    if( match_end_tag_open(parser) || match_less_than(parser) ) continue;
                    break;

                case '>':
                    if( match_greater_than(parser) ) continue;
                    break;


            };


            if( is_name_start_character(current_character) ){

                if( match_name(parser) ) continue;

            }

            //TODO: fail on invalid character (or create UNKNOWN token)
            //make sure to consume at least one or this loop will run forever
            consume_character( parser, 1 );

        }


    //Parsing: predict and parse each pattern according to the parsing rules
    //each parse_* function will construct an AST node (or several) from
    //the tokens that it uses to match to a rule

        token *current_token;
        parser->current_token_index = 0;

        //xml_element *root_element = malloc( sizeof(xml_element) );
        //wide_char root_name[4] = { 'r', 'o', 'o', 't' };
        //xml_element_create( root_element, root_name, 4 );
        //tree_node_create( parser->ast_root_node, root_element, &xml_element_destory_void );
        //parser->current_ast_node = parser->ast_root_node;
        //root_element->tree_node = parser->ast_root_node;

        while( (current_token = array_get_element_at(&parser->tokens, parser->current_token_index)) ){

            switch( current_token->type ){

                case TOKEN_TYPE_WHITESPACE:
                case TOKEN_TYPE_INVALID:
                case TOKEN_TYPE_UNKNOWN:
                    break;

                case TOKEN_TYPE_LESS_THAN:
                    if( parse_element_start_tag(parser) ) continue;
                    break;

                case TOKEN_TYPE_END_TAG_OPEN:
                    if( parse_element_end_tag(parser) ) continue;
                    break;

            };

            consume_token( parser, 1 );

            //printf( "%lu\n", parser->current_token_index );
            //token_print( current_token );

        }


}


static void print_xml_element( xml_element *element );

static int print_xml_element_by_node( void *node ){

    tree_node *tree_node = (struct tree_node*)node;

    xml_element *element = (xml_element*)tree_node->item;

    print_xml_element( element );

    return 0;

}


static void print_xml_element( xml_element *element ){

    tree_node *node = element->tree_node;
    size_t depth = tree_node_get_depth( node );

    size_t x;
    for( x = 0; x < depth; x++ ){
        printf( "    " );
    }

    size_t number_of_children = tree_node_count_children( node );

    printf( "<" );
    fwrite( element->tag_name, sizeof(wide_char), element->tag_name_length, stdout );
    printf( ">" );
    if( number_of_children > 0 ){
        printf( "\n" );
    }

        tree_node_visit_children( node, &print_xml_element_by_node );

    if( number_of_children > 0 ){
        for( x = 0; x < depth; x++ ){
            printf( "    " );
        }
    }
    printf( "</" );
    fwrite( element->tag_name, sizeof(wide_char), element->tag_name_length, stdout );
    printf( ">\n" );

}


void xml_parser_parse_from_file( xml_element *xml_element, xml_parser *parser, FILE *file ){

    xml_parser_tokenize_from_file( parser, file );

    printf( "There are %lu tokens.\n", array_size(&parser->tokens) );

    /*
    token current_token;
    while( array_pop(&parser->tokens, &current_token) ){

        token_print( &current_token );
        token_destroy( &current_token );

    }
    */

    xml_element = (struct xml_element*)(parser->ast_root_node->item);

    print_xml_element( xml_element );


}



