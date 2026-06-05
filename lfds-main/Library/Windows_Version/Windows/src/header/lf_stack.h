// Created by Nazli and Omar on 01.05.23.
//
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include <stdatomic.h>
//#include "FreeStructure.h"



#ifndef LOCKFREIESTACK_STACK_H
#define LOCKFREIESTACK_STACK_H

typedef struct _LfStackNode {
    int data;
    struct _LfStackNode* next;
    void (*free)(const struct _LfStackNode *); //free method
    _Atomic int count; //reference counter
}LfStackNode;

typedef struct _Stack {
    int tag;
    LfStackNode *top;
}Stack;



//implementation of reference counter
static inline void
ref_inc(const struct _LfStackNode *ref)
{
    atomic_fetch_add((_Atomic int *)&ref->count, 1); //increases reference counter
}

static inline void
ref_dec(const struct _LfStackNode *ref)
{
    if (atomic_fetch_sub((_Atomic int *)&ref->count, 1) <= 1){ //decreases ref counter and frees if 1
        free((void*)ref);
    }
}

static inline void
dec(const struct _LfStackNode *ref)
{
    atomic_fetch_sub((_Atomic int *)&ref->count, 1); //decreases ref counter
}


//--------------------------------3 versionen ------------------------

typedef struct stack_FreeNode {
    LfStackNode * data;
    struct stack_FreeNode* next;
}stackFreeNode;

typedef struct stack_FreeStack { //implement free list as stack
    stackFreeNode *top;
} stackFreeStack;

//--------------- stack funktionen ---
void stack_push(_Atomic Stack *stack, int value, struct stack_FreeStack* freeStack);
int stack_pop(_Atomic Stack *stack, struct stack_FreeStack* freeStack);
//-----------------------

void stack_free_push(stackFreeStack *freeStack, LfStackNode *data);
LfStackNode * stack_free_pop(stackFreeStack *freeStack);
void stack_retire(stackFreeStack *freeStack,LfStackNode *pointer);
LfStackNode * stack_reuse(stackFreeStack *freeStack);
void stack_freeAll(stackFreeStack *freeStack);





#endif //LOCKFREIESTACK_STACK_H
