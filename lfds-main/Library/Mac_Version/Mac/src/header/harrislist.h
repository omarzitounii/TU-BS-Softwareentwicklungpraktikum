//
// Created by keanukreed on 01.05.23.
//
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
//#include "FreeStructure.h"

#ifndef HARRISLIST_HARRISLIST_H
#define HARRISLIST_HARRISLIST_H

typedef struct harris_node harris_node_t;

struct harris_node{
    _Atomic int key;
    harris_node_t* _Atomic next;
    int count;
};

typedef struct list{
    harris_node_t* head;
    harris_node_t* tail;
}list_t;

//Helper functions
int initialise_list(list_t* list, harris_node_t* head, harris_node_t* tail);
void initialise_node(harris_node_t* tmp, int key);


void freeSet(list_t *list);

//----------------3 versionen ------------------
typedef struct harris_FreeNode {
    harris_node_t * data;
    struct harris_FreeNode* next;
}harrisFreeNode;

typedef struct harris_FreeStack { //implement free list as stack
    harrisFreeNode *top;
} harrisFreeStack;

//usable List functions -------------------------------------------
int insert(list_t* list, int key, struct harris_FreeStack* freeStack);
int delete(list_t* list, int key, struct harris_FreeStack* freeStack);
int find(list_t* list, int key, harrisFreeStack* freeStack);
void print_list(list_t* list);

//non usable list function
harris_node_t* search(list_t* list, harris_node_t **left, int key, harrisFreeStack* freeStack);
//---------------------------------------------------------------------
/*
typedef struct args_with_free_list_harris {
    harrisFreeStack* freeStack;
    list_t* set;
} args_with_free_list_harris_t;
 */

void harris_free_push(harrisFreeStack *freeStack, harris_node_t *data);
harris_node_t * harris_free_pop(harrisFreeStack *freeStack);
void harris_retire(harrisFreeStack *freeStack, harris_node_t *pointer);
harris_node_t* harris_reuse(harrisFreeStack *freeStack);
void harris_freeAll(harrisFreeStack *freeStack);

#endif //HARRISLIST_HARRISLIST_H

