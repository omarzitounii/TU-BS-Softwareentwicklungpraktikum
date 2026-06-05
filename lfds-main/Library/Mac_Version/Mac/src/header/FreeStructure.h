#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include "lf_stack.h"
#include "harrislist.h"
#include "M_S_Queue.h"
#include "benchmark.h"

/*
typedef struct _FreeNode {
    void * data;
    struct _FreeNode* next;
}FreeNode;

typedef struct _FreeStack { //implement free list as stack
    FreeNode *top;
} FreeStack;
/*

/*
typedef struct args_with_free_list {
    FreeStack* freeStack;
    queue_t * queue;
} args_with_free_list_t;
*/

/*
void free_push(FreeStack *freeStack, void *data);
void * free_pop(FreeStack *freeStack, enum data_structure structure);
void retire(FreeStack *freeStack,void *pointer);
void* reuse(FreeStack *freeStack);
void freeAll(FreeStack *freeStack);

*/