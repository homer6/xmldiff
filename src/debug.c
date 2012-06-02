#include <stdio.h>

#include "debug.h"


void print_bytes( void *address, size_t size ){

    unsigned char *p = (unsigned char *)address;
    size_t i;
    for( i = 0; i < size; ++i ){
        printf( "%02x", p[i] );
    }
    printf( "\n" );

}


