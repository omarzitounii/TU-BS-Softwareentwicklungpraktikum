#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#ifndef M_S_QUEUE_M_S_QUEUE_H
#define M_S_QUEUE_M_S_QUEUE_H

typedef struct {
    struct node_t * ptr;
    int count;
} pointer_t;

typedef int data_type;

typedef struct node_t {
    int value;
    _Atomic pointer_t next;
} node_t;

typedef struct {
    _Atomic pointer_t Head;
    _Atomic pointer_t Tail;
} queue_t;


typedef struct _FreeNode {
    node_t * data;
    struct _FreeNode* next;
}FreeNode;

typedef struct _FreeStack { //implement free list as stack
    FreeNode *top;
} FreeStack;

typedef struct args_with_free_list {
    FreeStack* freeStack;
    queue_t * queue;
} args_with_free_list_t;


void initialize(queue_t* Q);
void enqueue(queue_t* Q, data_type value, FreeStack *freeStack);
bool dequeue(queue_t *Q, data_type* pointer_value, FreeStack* freeStack);

void free_push(FreeStack *freeStack, node_t *data);
node_t * free_pop(FreeStack *freeStack);
void retire(FreeStack *freeStack,node_t *pointer);
node_t* reuse(FreeStack *freeStack);

void freeAll(FreeStack *freeStack);

#endif //M_S_QUEUE_M_S_QUEUE_H