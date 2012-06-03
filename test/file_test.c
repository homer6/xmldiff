#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "file_test.h"


static void run_ascii_test(){

    char *filename = "myfile775384905723345.txt";

    //create a temp file and close it
        char *file_contents = "Hi, I'm file contents.";
        FILE *file = fopen( filename, "w+" );
        fwrite( file_contents, sizeof(char), strlen(file_contents), file );
        fclose( file );

    //open the temp file for reading and use "file_get_contents"
        FILE *file2 = fopen( filename, "r+" );
        wide_char **string_ptr = malloc( sizeof(wide_char*) );
        file_get_contents( string_ptr, file2 );
        fclose( file2 );

    //ensure that it's the same as what I had put in
        //assert( strcmp(file_contents,*string_ptr) == 0 );

    //cleanup
        unlink( filename );
        free( *string_ptr );
        free( string_ptr );

}



static void run_unicode_test(){

    char *source_filename = "test/test_files/utf8_two_lines.txt";
    char *destination_filename = "test/test_files/utf8_two_lines_destination.txt";

    //open the source file for reading and use "file_get_contents"
        FILE *file1 = fopen( source_filename, "r+" );
        wide_char **string_ptr = malloc( sizeof(wide_char*) );
        size_t number_of_characters = file_get_contents( string_ptr, file1 );
        fclose( file1 );

    //open the destination file for writing and write the contents
        FILE *file2 = fopen( destination_filename, "w+" );
        if( file2 == NULL ){
            printf( "Could not open destination file for writing: %s", destination_filename );
            exit(5);
        }
        fwrite( *string_ptr, sizeof(wide_char), number_of_characters, file2 );
        fclose( file2 );

    //ensure that it's the same as what I had put in
        //assert( strcmp(file_contents,*string_ptr) == 0 );

    //cleanup
        free( *string_ptr );
        free( string_ptr );

}





void file_run_tests(){

    printf( "Running tests for file... " );
    fflush( stdout );

    //run_ascii_test();
    run_unicode_test();

    printf( "Done\n" );
    fflush( stdout );

}
