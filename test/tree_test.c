#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree_test.h"


static void delete_int( void *integer ){

    free( (int*)integer );

}


void tree_run_tests(){

    printf( "Running tests for tree... " );
    fflush( stdout );

    int y = 10;

    //create
        tree_node tree_node;
        tree_node_create( &tree_node, &y, NULL );


    //test add_child_first
        int elements[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int x;

        for( x = 0; x < 10; x++ ){

            tree_node_add_last_child( &tree_node, &elements[x] );

        }

        size_t number_of_children = tree_node_count_children(&tree_node);
        printf( "number of children: %lu\n", number_of_children );
        assert( number_of_children == 10 );


    //test array_pop
        /*
        int element;
        int elements_printed = 0;
        int expected_elements_printed = 9;
        while( array_pop(&array,&element) ){
            elements_printed++;
            //printf( "Element: %d\n", element );
        }

        assert( expected_elements_printed == elements_printed );
        */

    //test the destructor
        tree_node_destroy( &tree_node );



    printf( "Done\n" );
    fflush( stdout );

}
