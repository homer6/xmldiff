#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree_test.h"


static void delete_int( void *integer ){

    free( (int*)integer );

}


static int visit_tree_node( void *node ){

    tree_node *this_node = (tree_node*)node;
    int my_int = *(int *)(this_node->item);
    printf( "my node: %d\n", my_int );
    return 0;

}


void tree_run_tests(){

    printf( "Running tests for tree... " );
    fflush( stdout );

    int y = 10;

    //create
        tree_node tree_node;
        tree_node_create( &tree_node, &y, NULL );


    //test add_child_first
        int elements[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int x;

        for( x = 0; x < 10; x++ ){

            tree_node_add_last_child( &tree_node, &elements[x] );

        }

        int child_two = 80;
        int child_three = 90;
        int child_four = 100;

        struct tree_node *current_node, *secondary_node;

        current_node = tree_node_get_child_at( &tree_node, 2 );
        tree_node_add_last_child( current_node, &child_two );

        current_node = tree_node_get_child_at( &tree_node, 7 );
        secondary_node = tree_node_add_last_child( current_node, &child_three );

        tree_node_add_last_child( secondary_node, &child_four );




        size_t number_of_children = tree_node_count_children(&tree_node);
        printf( "number of children: %lu\n", number_of_children );
        assert( number_of_children == 10 );


    //test tree_node_visit_descendents_depth_first
        struct tree_node *found_node;
        found_node = tree_node_visit_descendents_depth_first( &tree_node, &visit_tree_node );

        if( found_node != NULL ){
            int found_int = *(int *)(found_node->item);
            printf( "found node: %d\n", found_int );
        }else{
            printf( "no node found\n" );
        }



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
