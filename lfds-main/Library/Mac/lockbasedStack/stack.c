//
// Created by Nazli and Omar on 22.05.23.
//

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void stack_push(Stack *stack, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("malloc");
        return;
    }
    new_node->data = value;

    pthread_mutex_lock(&stack->lock);
    new_node->next = stack->top;
    stack->top = new_node;
    pthread_mutex_unlock(&stack->lock);
}

int stack_pop(Stack *stack) {
    pthread_mutex_lock(&stack->lock);
    if (stack->top == NULL) {
        pthread_mutex_unlock(&stack->lock);
        return -1; // stack is empty
    }

    Node *top_node = stack->top;
    int value = top_node->data;
    stack->top = top_node->next;

    //printf("Popped value: %d\n", value);
    free(top_node);

    pthread_mutex_unlock(&stack->lock);


    return value;
}