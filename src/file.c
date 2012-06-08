#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/mman.h>

#include "file.h"



size_t file_get_contents( wide_char **destination, FILE *file ){

    const size_t constant_buffer_size = 1048560;
    char *buffer = malloc( constant_buffer_size );

    //initialize the string that will be returned
        size_t content_size = 0; // includes NULL
        size_t previous_content_size = content_size;
        //initial size is the same as the buffer size
        char *content = malloc( 1 );
        if( content == NULL ){
            perror( "Failed to allocate content" );
            exit( 1 );
        }
        //content[0] = '\0'; // make null-terminated


    size_t bytes_read;
    int x = 0;
    char *end_ptr;
    while( (bytes_read = fread(buffer, sizeof(char), constant_buffer_size, file)) ){

        char *old = content;

        x++;

        //calculate the new size and make room for it

            content_size += bytes_read;

            content = realloc( content, content_size + 1 );
            if( content == NULL ){
                perror( "Failed to reallocate content" );
                free( old );
                exit( 2 );
            }
            end_ptr = content + previous_content_size;

        //append the buffer to the string
        //strncat( content, buffer, bytes_read );
        memcpy( end_ptr, buffer, bytes_read );

        previous_content_size = content_size;

        //printf( "Bytes Read: %lu\n", bytes_read );
        //printf( "Content Size: %lu\n", content_size );

    }

    content[content_size] = '\0';

    //int fd = open("filename", O_RDONLY);
    //int fd = fileno( file );
    //size_t buffer_size = lseek( fd, 0, SEEK_END );
    //void *content = mmap( 0, buffer_size, PROT_READ, MAP_PRIVATE, fd, 0 );

    //return 1;

    if( ferror(file) ){
        free( content );
        free( buffer );
        perror( "Error reading from file." );
        exit( 3 );
    }

    //convert the mbstring to widecharacters    
        wide_char *temp_buffer = malloc( content_size * sizeof(wide_char) );

        setlocale(LC_ALL, "");
        size_t number_of_characters = mbstowcs( temp_buffer, content, content_size );
        if( number_of_characters == (size_t)-1 ){
            free( content );
            free( temp_buffer );
            free( buffer );
            perror( "Invalid multibyte character found." );
            exit( 4 );
        }

        //printf( "Content Size: %lu\n", content_size );
        //printf( "Number Characters: %lu\n", number_of_characters );

    //cleanup and return
        *destination = temp_buffer;
        free( content );
        free( buffer );
        return number_of_characters;

}
