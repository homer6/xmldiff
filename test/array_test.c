#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "array_test.h"


typedef struct person person;

struct person{
    int age;
    char *first_name;
    person *next_person;
};



static void delete_person_void( void *person ){



}



static void delete_int( void *integer ){

    free( (int*)integer );

}

static int foreach_test( void *integer ){

    printf( "number: %d\n", *(int*)integer );
    return 0;

}


void array_run_tests(){

    printf( "Running tests for array... " );
    fflush( stdout );

    //test array_create
        array array;
        array_create( &array, sizeof(int), 4, &delete_int );


    //test array_add
        int elements[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int x;

        for( x = 0; x < 9; x++ ){

            array_add( &array, &elements[x] );

        }

        assert( array.logical_length == 9 );
        assert( array.allocated_length == 16 );


    //array foreach
        array_foreach( &array, &foreach_test );


    //array get
        //get the third element
        int *element2 = array_get_element_at( &array, 2 );
        assert( element2 != NULL );
        assert( *element2 == 3 );


    //test array_pop
        int element;
        int elements_printed = 0;
        int expected_elements_printed = 9;
        while( array_pop(&array,&element) ){
            elements_printed++;
            //printf( "Element: %d\n", element );
        }

        assert( expected_elements_printed == elements_printed );


    //test array_resize
        array_resize( &array, 10000 );
        assert( array.allocated_length == 10000 );



    //test the destructor
        array_destroy( &array );



    printf( "Done\n" );
    fflush( stdout );

}
