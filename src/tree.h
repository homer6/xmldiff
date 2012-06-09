#ifndef XMLDIFF_TREE_H
#define XMLDIFF_TREE_H

#include "array.h"


/**
 * A tree is a generic tree structure (void*).
 *
 */

typedef struct tree_node tree_node;


 struct tree_node{
    tree_node *parent;
    tree_node *previous_sibling;
    tree_node *next_sibling;
    array children;
    void *item;
    void (*element_destructor)(void *element);
};


void tree_node_create( tree_node *tree_node, void *item, void (*element_destructor)(void *) );
void tree_node_destroy( tree_node *tree_node );



/**
 * Adds this child_element as the first child among this tree node's children.
 *
 */
//void tree_node_add_first_child( tree_node *tree_node, void *child_element );


/**
 * Adds this child_element as the last child among this tree node's children.
 *
 * Copies the contents of *element into the array.
 * Resizes the array, if necessary.
 *
 */
void tree_node_add_last_child( tree_node *tree_node, void *child_element );


/**
 * Returns the number of direct children underneath this node.
 *
 */
size_t tree_node_count_children( tree_node *tree_node );




#endif
