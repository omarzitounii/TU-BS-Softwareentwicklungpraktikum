// Created by Nazli and Omar on 01.05.23.
//
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include <stdatomic.h>



#ifndef LOCKFREIESTACK_STACK_H
#define LOCKFREIESTACK_STACK_H

typedef struct _Node {
    int data;
    struct _Node* next;
    void (*free)(const struct _Node *); //free method
    _Atomic int count; //reference counter
}Node;

typedef struct _Stack {
    int tag;
    Node *top;
}Stack;

typedef struct _FreeNode { //free list node struct
    Node* data;
    struct _FreeNode* next;
}FreeNode;

typedef struct _FreeStack { //implement free list as stack
    FreeNode *top;
} FreeStack;


//implementation of reference counter
static inline void
ref_inc(const struct _Node *ref)
{
    atomic_fetch_add((_Atomic int *)&ref->count, 1); //increases reference counter
}

static inline void
ref_dec(const struct _Node *ref)
{
    if (atomic_fetch_sub((_Atomic int *)&ref->count, 1) <= 1){ //decreases ref counter and frees if 1
        free((void*)ref);
    }
}

static inline void
dec(const struct _Node *ref)
{
    atomic_fetch_sub((_Atomic int *)&ref->count, 1); //decreases ref counter
}

void stack_push(_Atomic Stack *stack, int value, FreeStack* freeStack);
int stack_pop(_Atomic Stack *stack, FreeStack* freeStack);

void free_push(FreeStack *freeStack, Node *data);
Node* free_pop(FreeStack *freeStack);
void retire(FreeStack *freeStack,Node *pointer);
Node* reuse(FreeStack *freeStack);

void freeAll(FreeStack *freeStack);






#endif //LOCKFREIESTACK_STACK_H