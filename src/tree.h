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
 * Returns the address of the new tree_node that wraps around this child_element.
 *
 */
tree_node *tree_node_add_last_child( tree_node *tree_node, void *child_element );


/**
 * Returns the number of direct children underneath this node.
 *
 */
size_t tree_node_count_children( tree_node *tree_node );



/**
 * Iterates through each of the child items and calls the supplied function.
 * If any of the calls to visitor_function return -1, then the iteration will
 * stop.
 *
 */
void tree_node_visit_children( tree_node *tree_node, int (*visitor_function)(void *) );



/**
 * Iterates through each of the child items (and their children) recursively
 * and calls the supplied function. If any of the calls to visitor_function
 * return -1, then the iteration will stop.
 *
 * This function will return the address of the tree_node that it stopped at.
 *
 */
tree_node *tree_node_visit_descendents_depth_first( tree_node *tree_node, int (*visitor_function)(void *) );



/**
 * Iterates through each of the child items (and their children) recursively
 * and calls the supplied function. If any of the calls to visitor_function
 * return -1, then the iteration will stop.
 *
 * This function will return the address of the element that it stopped at.
 *
 */
//void *tree_node_visit_descendents_breadth_first( tree_node *tree_node, int (*visitor_function)(void *) );



/**
 * Gets the address of the nth child of tree_node.
 * Returns NULL if there is no child at that index.
 * Index 0 gets the first child.
 */
tree_node *tree_node_get_child_at( tree_node *tree_node, size_t index );





#endif
