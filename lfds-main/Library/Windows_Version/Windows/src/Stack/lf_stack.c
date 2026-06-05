//
// Created by Nazli and Omar  on 04.05.23.
//

#include "../header/lf_stack.h"

void stack_push(_Atomic Stack *stack, int value, struct stack_FreeStack *freeStack)
{
    Stack new_stack, old_stack;
    LfStackNode *new_node = stack_reuse(freeStack);//allocate memory for the new node
    //new_node->count = 0; //initialise the reference counter
    //ref_inc(new_node);
    if (new_node == NULL) { //check if allocation succeed
        perror("malloc");
    }
    new_node->data = value;//set the data of the new node
    do{
        old_stack= atomic_load(stack);//2 local Stack Objects
        new_node->next = old_stack.top;//Let the next pointer of new node points, where the top of old_stack points.
        new_stack.top = new_node;//Let the top pointer of new_stack points on new_node
        new_stack.tag =  old_stack.tag+1;
    }while(!atomic_compare_exchange_weak(stack, &old_stack, new_stack));//if old_stack is still consistent to the global Stack. Apply the local modification globally
}


int stack_pop(_Atomic Stack *stack, struct stack_FreeStack* freeStack)
{
    Stack new_stack, old_stack; //2 local Stack Objects
    do{
        old_stack = atomic_load(stack); //in old_stack read the actual value of the global Stack
        if(old_stack.top == NULL) //return -1 when the stack is empty
        {
            return -1;
        }
        //ref_inc(old_stack.top);
        new_stack.top = old_stack.top->next; //let the top pointer of new_stack points on the next node of the top of old_stack
        new_stack.tag = old_stack.tag+1;
        //dec(old_stack.top);
    }while(!atomic_compare_exchange_strong(stack, &old_stack, new_stack)); //if old_stack is still consistent to the global Stack. Apply the local modification globally
    stack_retire(freeStack, old_stack.top); //add address of old top of stack to the free list
    //ref_dec(old_stack.top); // decrement the reference counter and free the poped node if it's safe to free
    return 0;
}



//---------------------------------------------3 versionen ----------------------------------
void stack_free_push(stackFreeStack *freeStack, LfStackNode* data) {//push pointer  to the free list
    stackFreeNode* node = malloc(sizeof(stackFreeNode));
    node->data = data;
    node->next = freeStack->top;

    freeStack->top = node;
}

LfStackNode *stack_free_pop(stackFreeStack *freeStack) { //take a adress from the free list
    if(freeStack->top == NULL) { //check if list empty
        return malloc(sizeof(LfStackNode));
    }
    stackFreeNode *result = freeStack->top;
    freeStack->top = freeStack->top->next;
    LfStackNode* ret = result->data;
    free(result);
    return ret;


}

void stack_freeAll(stackFreeStack *freeStack) { //delete all nodes at stack and free stack
    while(freeStack->top != NULL) {
        stackFreeNode * temp = freeStack->top;
        freeStack->top = freeStack->top->next;
        free(temp->data);
        free(temp);
    }

}

void stack_retire(stackFreeStack *freeStack, LfStackNode *pointer) { //add Node to the list to be later used
    stack_free_push(freeStack,pointer);
}


LfStackNode *stack_reuse(stackFreeStack *freeStack) { //free node
    LfStackNode *result = stack_free_pop(freeStack);
    return result;
}



