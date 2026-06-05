//
// Created by Omar and Mohamed Amine on 16.06.2023.
//

#include <stdlib.h>
#include "lb_set.h"
#include <errno.h>

/*Implemented lock based set in form of binary tree
 * Initialization in benchmark.c should be like that: Set_Node* tree = NULL;
 * by insert: tree = set_insert(tree, j);
 * by delete: tree = set_delete(tree,j);
 */

/* set_search not needed for benchmark
Set_Node *set_search(Set_Node *tree, int key) {
    Set_Node *current = tree;
    while (current != NULL)
    {
        if (current->value == key)
        {
            return current;
        }
        if (current->value > key)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

    }
    return NULL;
}
 */

Set_Node* set_insert(Set_Node* tree, int value)
{
    if (tree == NULL)
    {

        Set_Node* newNode = (Set_Node*)malloc(sizeof(Set_Node));
        if (newNode == NULL){
            perror(malloc);
        }
        newNode->value = value;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (value < tree->value)
    {
        tree->left = set_insert(tree->left, value);
    }
    else
    {
        tree->right = set_insert(tree->right, value);
    }
    return tree;
}

Set_Node* find_min(Set_Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Set_Node* set_delete(Set_Node* tree, int key) {
    if (tree == NULL) {
        return NULL;
    }
    if (key < tree->value) {
        tree->left = set_delete(tree->left, key);
    } else if (key > tree->value) {
        tree->right = set_delete(tree->right, key);
    } else {
        // Key found, perform deletion

        if (tree->left == NULL && tree->right == NULL) {
            // Case 1: Set_Node has no children
            free(tree);
            return NULL;
        } else if (tree->left == NULL) {
            // Case 2: Set_Node has only right child
            Set_Node* temp = tree->right;
            free(tree);
            return temp;
        } else if (tree->right == NULL) {
            // Case 3: Set_Node has only left child
            Set_Node* temp = tree->left;
            free(tree);
            return temp;
        } else {
            // Case 4: Set_Node has both left and right children
            Set_Node* min_right = find_min(tree->right);
            tree->value = min_right->value;
            tree->right = set_delete(tree->right, min_right->value);
        }
    }
    return tree;
}

