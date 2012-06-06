#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "file.h"



size_t file_get_contents( wide_char **destination, FILE *file ){

    char buffer[BUF_SIZE];

    //initialize the string that will be returned
        size_t content_size = 1; // includes NULL
        //initial size is the same as the buffer size
        char *content = malloc( content_size );
        if( content == NULL ){
            perror( "Failed to allocate content" );
            exit( 1 );
        }
        content[0] = '\0'; // make null-terminated

    size_t buffer_size;
    while( fgets(buffer, BUF_SIZE, file) ){

        char *old = content;

        //calculate the new size and make room for it
            buffer_size = strlen( buffer );
            if( buffer_size > BUF_SIZE ){
                buffer_size = BUF_SIZE;
            }
            content_size += buffer_size;
            content = realloc( content, content_size );
            if( content == NULL ){
                perror( "Failed to reallocate content" );
                free( old );
                exit( 2 );
            }

        //append the buffer to the string
        strncat( content, buffer, buffer_size );

    }

    if( ferror(file) ){
        free( content );
        perror( "Error reading from file." );
        exit( 3 );
    }

    //convert the mbstring to widecharacters
        wide_char *temp_buffer = malloc( content_size * sizeof(wide_char) + 1 );

        setlocale(LC_ALL, "");
        size_t number_of_characters = mbstowcs( temp_buffer, content, content_size );
        if( number_of_characters == (size_t)-1 ){
            free( content );
            free( temp_buffer );
            perror( "Invalid multibyte character found." );
            exit( 4 );
        }

    //cleanup and return
        *destination = temp_buffer;
        free( content );
        return number_of_characters;

}
