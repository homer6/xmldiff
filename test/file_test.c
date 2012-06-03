#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "file_test.h"




void file_run_tests(){

    printf( "Running tests for file... " );
    fflush( stdout );

    char *filename = "myfile775384905723345.txt";

    //create a temp file and close it
        char *file_contents = "Hi, I'm file contents.";
        FILE *file = fopen( filename, "w+" );
        fwrite( file_contents, sizeof(char), strlen(file_contents), file );
        fclose( file );

    //open the temp file for reading and use "file_get_contents"
        FILE *file2 = fopen( filename, "r+" );
        char **string_ptr = malloc( sizeof(char*) );
        file_get_contents( string_ptr, file2 );
        fclose( file2 );

    //ensure that it's the same as what I had put in
        assert( strcmp(file_contents,*string_ptr) == 0 );

    //cleanup
        unlink( filename );
        free( *string_ptr );
        free( string_ptr );

    printf( "Done\n" );
    fflush( stdout );

}
