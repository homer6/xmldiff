#include <string.h>
#include <stdlib.h>

#include "array.h"
#include "error.h"



array *array_create( size_t element_size, size_t initial_allocated_size, void (*element_destructor)(void *element) ){

    array *new_array;
    new_array = malloc( sizeof(array) );
    if( new_array == NULL ){
        error_fatal( "Could not allocate memory for array." );
    }

    new_array->elements = malloc( element_size * initial_allocated_size );
    if( new_array->elements == NULL ){
        error_fatal( "Could not allocate memory for array elements." );
    }

    new_array->element_size = element_size;
    new_array->element_destructor = element_destructor;
    new_array->logical_length = 0;
    new_array->allocated_length = initial_allocated_size;

    return new_array;

}



void array_destroy( array *array ){

    size_t x;

    if( array->element_destructor != NULL ){

        for( x = 0; x < array->logical_length; x++ ){
            //use the char* hack to free the element at the given offset from array->elements
            array->element_destructor( (char *)array->elements + x * array->element_size );
        }

    }

    free( array->elements );
    free( array );

}



static void array_grow( array *array ){

    array->allocated_length *= 2;
    array->elements = realloc( array->elements, array->allocated_length * array->element_size );
    if( array->elements == NULL ){
        error_fatal( "Could not reallocate memory for array elements." );
    }

}



void array_add( array *array, void *element ){

    //allocate more memory, if needed
    if( array->logical_length == array->allocated_length ){
        array_grow( array );
    }

    void *destination = (char*)array->elements + array->logical_length * array->element_size;
    memcpy( destination, element, array->element_size );
    array->logical_length++;

}



int array_pop( array *array, void *element ){

    if( array->logical_length == 0 ){
        return 0;
    }

    void *source = (char*)array->elements + ( array->logical_length - 1 ) * array->element_size;
    memcpy( element, source, array->element_size );
    array->logical_length--;

    return 1;

}




