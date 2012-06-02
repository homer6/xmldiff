#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"



int main( int argc, char **argv ){


    if( argc != 3 ){
        printf( "Usage: %s file1.xml file2.xml\n", argv[0] );
        return 0;
    }

    FILE *left_file = fopen( argv[1], "rb" );
    char *left_file_contents = file_get_contents( left_file );
    fclose( left_file );


    FILE *right_file = fopen( argv[1], "rb" );
    char *right_file_contents = file_get_contents( right_file );
    fclose( right_file );


    char fatty[6];
    strncat( fatty, left_file_contents, 5 );

    printf( "STDIN: %s", fatty );


    free( left_file_contents );
    free( right_file_contents );

    return 0;

}
