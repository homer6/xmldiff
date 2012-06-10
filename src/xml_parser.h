#ifndef XMLDIFF_XML_PARSER_H
#define XMLDIFF_XML_PARSER_H

#include <stdio.h>

#include "token.h"
#include "array.h"
#include "xml_element.h"
#include "tree.h"



typedef struct xml_parser{
    wide_char *file_contents;
    wide_char *current_position;
    wide_char *end_of_file;
    size_t file_contents_length;
    struct array tokens;
    size_t current_token_index;
    tree_node *ast_root_node;
    tree_node *current_ast_node;
}xml_parser;


void xml_parser_create( xml_parser *parser, size_t initial_token_array_size );
void xml_parser_destroy( xml_parser *parser );

void xml_parser_parse_from_file( xml_element *xml_element, xml_parser *parser, FILE *file );





#endif
