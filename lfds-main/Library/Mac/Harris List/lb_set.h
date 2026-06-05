//
// Created by Omar and Mohamed Amine on 16.06.2023
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*Implemented lock based set in form of binary tree
 * Initialization in benchmark.c should be like that: Set_Node* tree = NULL;
 * by insert: tree = set_insert(tree, j);
 * by delete: tree = set_delete(tree,j);
 */

typedef struct _set_node {
    struct _set_node *left;
    struct _set_node *right;
    int value;
}Set_Node;

//Set_Node* set_search(Set_Node *tree, int key);
Set_Node* set_insert(Set_Node* tree, int value);
Set_Node* find_min(Set_Node* node);
Set_Node* set_delete(Set_Node* tree, int key);
