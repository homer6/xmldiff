#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "error.h"


static void tree_node_void_destroy( void* tree_node ){

    tree_node_destroy( (struct tree_node*)tree_node );

}


void tree_node_create( tree_node *tree_node, void *item, void (*element_destructor)(void *) ){

    array_create( &tree_node->children, sizeof(struct tree_node*), 10, &tree_node_void_destroy );
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



void tree_node_add_last_child( tree_node *tree_node, void *child_element ){

    struct tree_node *child_tree_node = malloc( sizeof(struct tree_node) );
    tree_node_create( child_tree_node, child_element, tree_node->element_destructor );

    child_tree_node->parent = tree_node;
    array_add( &tree_node->children, child_tree_node );

    size_t number_of_children = tree_node->children.logical_length;

    if( number_of_children == 1 ){
        //this is the only child, don't worry about updating the sibling pointers
    }else{
        struct tree_node *previous_last_child = (struct tree_node*)( (char*)tree_node->children.elements + (sizeof(struct tree_node*) * (number_of_children - 1)) );

        previous_last_child->next_sibling = child_tree_node;
        child_tree_node->previous_sibling = previous_last_child;

    }

}


size_t tree_node_count_children( tree_node *tree_node ){

    return tree_node->children.logical_length;

}


