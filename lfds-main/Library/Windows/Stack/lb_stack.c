//
// Created by Keanu Wirz on 23.05.23.
//

#include "lb_stack.h"

//implementation of lock based stack

void lb_push(lb_stack_t* stack, int val){
    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    if(lb_isEmpty(stack)){
        ptr->val = val;
        ptr->next = NULL;
        stack->head = ptr;
    }
    else{
        ptr->val = val;
        ptr->next = stack->head;
        stack->head=ptr;
    }
}

int lb_pop(lb_stack_t* stack){
    int item;
    struct node *ptr;
    if (!lb_isEmpty(stack)){
        item = stack->head->val;
        ptr = stack->head;
        stack->head = stack->head->next;
        free(ptr);
        return item;
    }
    return -1;
}

int lb_peek(lb_stack_t* stack){
    int x = stack->head->val;
    return x;
}

bool lb_isEmpty(lb_stack_t* stack){
    if(stack->head == NULL){
        return true;
    }
    return false;
}
