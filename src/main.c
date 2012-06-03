#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "xml_parser.h"
#include "xml_element.h"


//main executable

int main( int argc, char **argv ){

    if( argc != 3 ){
        printf( "Usage: %s file1.xml file2.xml\n", argv[0] );
        return 0;
    }


    xml_element left_xml_element, right_xml_element;
    struct stat left_file_stat, right_file_stat;
    xml_parser left_xml_parser, right_xml_parser;


    //this program relies on these assertions being true
    assert( sizeof(off_t) == sizeof(size_t) );
    assert( sizeof(wchar_t) == 4 );


    FILE *left_file = fopen( argv[1], "rb" );    
    if( left_file == NULL ){
        printf( "Error opening %s\n", argv[1] );
        return 1;
    }
    stat( argv[1], &left_file_stat );
    xml_parser_create( &left_xml_parser, left_file_stat.st_size );
    xml_parser_parse_from_file( &left_xml_element, &left_xml_parser, left_file );
    fclose( left_file );



    FILE *right_file = fopen( argv[2], "rb" );
    if( right_file == NULL ){
        printf( "Error opening %s\n", argv[2] );
        return 1;
    }
    stat( argv[2], &right_file_stat );
    xml_parser_create( &right_xml_parser, right_file_stat.st_size );
    xml_parser_parse_from_file( &right_xml_element, &right_xml_parser, right_file );
    fclose( right_file );



    xml_element_destroy( &left_xml_element );
    xml_element_destroy( &right_xml_element );
    xml_parser_destroy( &left_xml_parser );
    xml_parser_destroy( &right_xml_parser );

    return 0;

}
