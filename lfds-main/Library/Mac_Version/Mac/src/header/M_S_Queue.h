#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
//#include "FreeStructure.h"

#ifndef M_S_QUEUE_M_S_QUEUE_H
#define M_S_QUEUE_M_S_QUEUE_H

typedef struct {
    struct ms_queue_node_t * ptr;
    int count;
} pointer_t;

typedef int data_type;

typedef struct ms_queue_node_t {
    int value;
    _Atomic pointer_t next;
} ms_queue_node_t;

typedef struct {
    _Atomic pointer_t Head;
    _Atomic pointer_t Tail;
} queue_t;

//--------------------------------------3 Versionen von diesen FREE STRUCT UND FUNKTIONEN-----------------------
typedef struct msqueue_FreeNode {
    ms_queue_node_t * data;
    struct msqueue_FreeNode* next;
}msqueueFreeNode;

typedef struct msqueue_FreeStack { //implement free list as stack
    msqueueFreeNode *top;
} msqueueFreeStack;

//------------------ queue funktionen------
void initialize(queue_t* Q);
void enqueue(queue_t* Q, data_type value, msqueueFreeStack *freeStack);
bool dequeue(queue_t *Q, data_type* pointer_value, msqueueFreeStack* freeStack);
//-------------------------------

/*
typedef struct args_with_free_list_msqueue {
    msqueueFreeStack* freeStack;
    queue_t * queue;
} args_with_free_list_msqueue_t;
 */

void msqueue_free_push(msqueueFreeStack *freeStack, ms_queue_node_t *data);
ms_queue_node_t * msqueue_free_pop(msqueueFreeStack *freeStack);
void msqueue_retire(msqueueFreeStack *freeStack,ms_queue_node_t *pointer);
ms_queue_node_t* msqueue_reuse(msqueueFreeStack *freeStack);
void msqueue_freeAll(msqueueFreeStack *freeStack);




#endif //M_S_QUEUE_M_S_QUEUE_H

