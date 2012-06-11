#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "error.h"


static void tree_node_child_destroy( void* tree_node ){

    struct tree_node *child = *(struct tree_node**)tree_node;
    tree_node_destroy( child );
    free( child );

}


void tree_node_create( tree_node *tree_node, void *item, void (*element_destructor)(void *) ){

    array_create( &tree_node->children, sizeof(struct tree_node*), 10, &tree_node_child_destroy );
    tree_node->item = item;
    tree_node->next_sibling = NULL;
    tree_node->previous_sibling = NULL;
    tree_node->parent = NULL;
    tree_node->element_destructor = element_destructor;

}


void tree_node_destroy( tree_node *tree_node ){

    if( tree_node->element_destructor != NULL ){
        tree_node->element_destructor( tree_node->item );
    }
    array_destroy( &tree_node->children );

}


tree_node *tree_node_add_last_child( tree_node *tree_node, void *child_element ){

    struct tree_node *child_tree_node = malloc( sizeof(struct tree_node) );
    tree_node_create( child_tree_node, child_element, tree_node->element_destructor );

    child_tree_node->parent = tree_node;
    array_add( &tree_node->children, &child_tree_node );

    size_t number_of_children = tree_node->children.logical_length;

    if( number_of_children == 1 ){
        //this is the only child, don't worry about updating the sibling pointers
    }else{

        struct tree_node *previous_last_child = (struct tree_node*)( (char*)tree_node->children.elements + (sizeof(struct tree_node*) * (number_of_children - 2)) );

        previous_last_child->next_sibling = child_tree_node;
        child_tree_node->previous_sibling = previous_last_child;

    }

    return child_tree_node;

}


size_t tree_node_count_children( tree_node *tree_node ){

    return tree_node->children.logical_length;

}


/*
static void *walk_children_depth_first( int (*visitor_function)(void *), void* child ){




}

*/


tree_node *tree_node_get_child_at( tree_node *tree_node, size_t index ){

    void *current_element;
    struct tree_node *current_node;

    if( index < tree_node->children.logical_length ){

        current_element = (char *)tree_node->children.elements + tree_node->children.element_size * index;
        current_node = *(struct tree_node**)current_element;
        return current_node;

    }

    return NULL;

}


size_t tree_node_get_depth( tree_node *tree_node ){

    size_t hops = 0;
    struct tree_node *current_node = tree_node;

    while( current_node->parent ){
        current_node = current_node->parent;
        hops++;
    }

    return hops;

}


tree_node *tree_node_visit_children( tree_node *tree_node, int (*visitor_function)(void *) ){

    if( tree_node->children.logical_length == 0 ){
        return NULL;
    }

    size_t x;
    void *current_element;
    struct tree_node *current_node;

    for( x = 0; x < tree_node->children.logical_length; x++ ){

        //could substitute tree_node_get_child_at here
        current_element = (char *)tree_node->children.elements + tree_node->children.element_size * x;
        current_node = *(struct tree_node**)current_element;

        if( visitor_function(current_node) == -1 ){
            return current_node;
        }

    }

    return NULL;

}


tree_node *tree_node_visit_descendents_depth_first( tree_node *tree_node, int (*visitor_function)(void *) ){

    if( tree_node->children.logical_length == 0 ){
        return NULL;
    }

    size_t x;
    void *current_element;
    struct tree_node *current_node;
    struct tree_node *return_value;

    for( x = 0; x < tree_node->children.logical_length; x++ ){

        //could substitue tree_node_get_child_at here
        current_element = (char *)tree_node->children.elements + tree_node->children.element_size * x;
        current_node = *(struct tree_node**)current_element;

        if( visitor_function(current_node) == -1 ){
            return current_node;
        }

        return_value = tree_node_visit_descendents_depth_first( current_node, visitor_function );
        if( return_value != NULL ){
            return return_value;
        }

    }

    return NULL;

}


/*
void *tree_node_visit_descendents_breadth_first( tree_node *tree_node, int (*visitor_function)(void *) ){



}
*/


