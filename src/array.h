#ifndef XMLDIFF_ARRAY_H
#define XMLDIFF_ARRAY_H


/**
 * An array is a generic linear store (void*) that resizes automatically, if necessary.
 *
 * If an element destructor is provided to the constructor, the store assumes ownership
 * of the elements added
 *
 * I'm calling this an array, but others might have called it a stack.
 */



typedef struct array{
    size_t logical_length;
    size_t allocated_length;
    size_t element_size;
    void *elements;
    //void *(*element_copy_constructor)(void *element);
    void (*element_destructor)(void *element);
}array;


void array_create( array *array, size_t element_size, size_t initial_allocated_size, void (*element_destructor)(void *element) );
void array_destroy( array *array );


/**
 * Adds an element to this array.
 *
 * Copies the contents of *element into the array.
 * Resizes the array, if necessary.
 *
 */
void array_add( array *array, void *element );



/**
 * Gets an element from the end of this array and removes it from the array.
 *
 * Copies the contents of the popped element into *element.
 *
 * Returns 1 on success. Returns 0 on empty array (*element unmodified).
 */
int array_pop( array *array, void *element );



/**
 * Resizes an array's allocated space to the new_size.
 *
 * This size must be greater than the current size.
 */
void array_resize( array *array, size_t new_size );



/**
 * Gets the size of an array.
 *
 */
size_t array_size( array *array );



/**
 * Starting from the beginning of the array, iterates through each of the
 * items and calls the supplied function. If any of the calls to
 * visitor_function return -1, then the foreach loop will stop iterating
 * and return the address of the element that it stopped at.
 *
 * If there are no elements in the array or none of the calls to
 * visitor_function returned -1, this function will return NULL.
 */
void *array_foreach( array *array, int (*visitor_function)(void *) );




/**
 * Gets a void * pointer to the element at the provided index.
 * Returns NULL if not found.
 *
 */
void *array_get_element_at( array *array, size_t index );



#endif
