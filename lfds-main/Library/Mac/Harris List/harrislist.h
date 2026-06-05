//
// Created by keanukreed on 01.05.23.
//
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef HARRISLIST_HARRISLIST_H
#define HARRISLIST_HARRISLIST_H

typedef struct node node_t;

struct node{
    _Atomic int key;
    node_t* _Atomic next;
    int count;
};

typedef struct list{
    node_t* head;
    node_t* tail;
}list_t;

typedef struct _FreeNode { //free list node struct
    node_t* data;
    struct _FreeNode* next;
}FreeNode;

typedef struct _FreeStack { //implement free list as stack
    FreeNode *top;
} FreeStack;

typedef struct args_with_free_list {
    FreeStack* freeStack;
    list_t* set;
} args_with_free_list_t;





//useable List functions
int insert(list_t* list, int key, FreeStack* freeStack);
int delete(list_t* list, int key, FreeStack* freeStack);
int find(list_t* list, int key, FreeStack* freeStack);
void print_list(list_t* list);

//non useable list function
node_t* search(list_t* list, node_t **left, int key, FreeStack* freeStack);

//Helper functions
int initialise_list(list_t* list, node_t* head, node_t* tail);
void initialise_node(node_t* tmp, int key);

void free_push(FreeStack *freeStack, node_t *data);
node_t* free_pop(FreeStack *freeStack);
void retire(FreeStack *freeStack, node_t *pointer);
node_t* reuse(FreeStack *freeStack);

void freeAll(FreeStack *freeStack);
void freeSet(list_t *list);

#endif //HARRISLIST_HARRISLIST_H
