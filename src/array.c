#include <string.h>
#include <stdlib.h>

#include "array.h"
#include "error.h"



void array_create( array *array, size_t element_size, size_t initial_allocated_size, void (*element_destructor)(void *element) ){

    array->elements = malloc( element_size * initial_allocated_size );
    if( array->elements == NULL ){
        error_fatal( "Could not allocate memory for array elements." );
    }

    array->element_size = element_size;
    array->element_destructor = element_destructor;
    array->logical_length = 0;
    array->allocated_length = initial_allocated_size;

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

}



void array_resize( array *array, size_t new_size ){

    if( array->allocated_length >= new_size ){
        error_fatal( "Could not resize array to be smaller or equal in size." );
    }

    array->allocated_length = new_size;

    array->elements = realloc( array->elements, array->allocated_length * array->element_size );
    if( array->elements == NULL ){
        error_fatal( "Could not reallocate memory for array elements." );
    }

}



void array_add( array *array, void *element ){

    //allocate more memory, if needed
    if( array->logical_length == array->allocated_length ){
        //double the size of the array
        array_resize( array, array->allocated_length * 2 );
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




