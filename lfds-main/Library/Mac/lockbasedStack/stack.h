//
// Created by Nazli and Omar on 22.05.23.
//

#ifndef LOCKBASEDDATENSTRUKTUR_STACK_H
#define LOCKBASEDDATENSTRUKTUR_STACK_H
#include <pthread.h>

typedef struct _Node {
    int data;
    struct _Node* next;
} Node;

typedef struct _Stack {
    Node *top;
    pthread_mutex_t lock;
} Stack;




void stack_push(Stack *stack, int value);
int stack_pop(Stack *stack);
#endif //LOCKBASEDDATENSTRUKTUR_STACK_H
