#include <string.h>
#include <stdlib.h>

#include "token.h"
#include "error.h"


void token_create( token *token, int type, const char *contents ){

    size_t content_length = strlen( contents );

    token->contents = malloc( content_length + 1 );
    if( token->contents == NULL ){
        error_fatal( "Could not allocate memory for token contents." );
    }
    strcpy( token->contents, contents );

    token->type = type;

}



void token_destroy( token *token ){

    free( token->contents );

}



