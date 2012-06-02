#include <stdio.h>


#include <stdlib.h>

#include "error.h"


void error_fatal( const char *error_message ){

    printf( "Fatal Error: %s\n", error_message );
    exit(2);

}
