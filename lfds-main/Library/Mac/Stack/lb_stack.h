//
// Created by Keanu Wirz on 23.05.23.
//

#ifndef LOCKFREIESTACK_LB_STACK_H
#define LOCKFREIESTACK_LB_STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Lock based variant of stack data structure

//the node with value and next
struct node{
    int val;
    struct node *next;
};

//the stack itself with only the head pointer
typedef struct lb_stack{
    struct node* head;
}lb_stack_t;

void lb_push(lb_stack_t* stack, int val);       // Push element to the top of the stack
int lb_pop(lb_stack_t* stack);         // Remove and return the top most element of the stack
int lb_peek(lb_stack_t* stack);        // Return the top most element of the stack
bool lb_isEmpty(lb_stack_t* stack);    // Check if the stack is in Underflow state or not

#endif //LOCKFREIESTACK_LB_STACK_H
