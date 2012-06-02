#include <string.h>
#include <stdlib.h>

#include "token.h"
#include "error.h"


token *token_create( int type, const char *contents ){

    token *token;
    token = malloc( sizeof(token) );
    if( token == NULL ){
        error_fatal( "Could not allocate memory for token." );
    }

    size_t content_length = strlen( contents );

    token->contents = malloc( content_length + 1 );
    if( token->contents == NULL ){
        error_fatal( "Could not allocate memory for token contents." );
    }

    token->type = type;

    return token;

}



void token_destroy( token *token ){

    free( token->contents );
    free( token );

}



