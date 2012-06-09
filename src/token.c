#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

#include "token.h"
#include "error.h"




void token_create( token *token, int type, wide_char *start_position, size_t contents_length ){

    assert( contents_length > 0 );

    //add the other members
        token->type = type;
        token->start_position = start_position;
        token->end_position = start_position + contents_length;

}



void token_destroy( token *token ){

    //to get rid of the compiler warning
    token = token;

}



void token_print( token *token ){

    printf( "Token: " );
    char type_name[100];

    switch( token->type ){

        case TOKEN_TYPE_INVALID:
            strcpy( type_name, "Invalid" );
            break;

        case TOKEN_TYPE_NAME:
            strcpy( type_name, "Name" );
            break;

        case TOKEN_TYPE_WHITESPACE:
            strcpy( type_name, "Whitespace" );
            break;

        case TOKEN_TYPE_LESS_THAN:
            strcpy( type_name, "Less Than" );
            break;

        case TOKEN_TYPE_GREATER_THAN:
            strcpy( type_name, "Greater Than" );
            break;

        case TOKEN_TYPE_UNKNOWN:
            strcpy( type_name, "Unknown" );
            break;

        default:
            strcpy( type_name, "Not listed in print function" );

    };

    printf( "type: %s  ", type_name );

    printf( "\"" );
    fwrite( token->start_position, sizeof(wide_char), token->end_position - token->start_position, stdout );
    printf( "\"" );

    printf( "\n" );


}




