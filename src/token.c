#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

#include "token.h"
#include "error.h"




void token_create( token *token, int type, const wide_char *contents, size_t contents_length ){

    assert( contents_length > 0 );

    //copy string contents (without a null terminator)
        size_t allocated_size = contents_length * sizeof(wide_char);
        token->contents = malloc( allocated_size );
        if( token->contents == NULL ){
            error_fatal( "Could not allocate memory for token contents." );
        }
        memcpy( token->contents, contents, allocated_size );

    //add the other members
        token->type = type;
        token->contents_length = contents_length;

}



void token_destroy( token *token ){

    free( token->contents );

}



void token_print( token *token ){

    printf( "Token: " );
    fwrite( token->contents, sizeof(wide_char), token->contents_length, stdout );
    printf( "\n" );


}




