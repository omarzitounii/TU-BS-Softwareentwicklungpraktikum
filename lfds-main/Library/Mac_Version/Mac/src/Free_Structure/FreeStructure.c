
#include "../header/FreeStructure.h"

/*
void free_push(FreeStack *freeStack, void* data) {//push pointer  to the free list
    FreeNode* node = malloc(sizeof(FreeNode));
    node->data = data;
    node->next = freeStack->top;

    freeStack->top = node;
}

void *free_pop(FreeStack *freeStack, enum data_structure structure) { //take a adress from the free list
    if(freeStack->top == NULL) { //check if list empty
        if (structure == STACK){
            return malloc(sizeof(LfStackNode));
        }

    }
    FreeNode *result = freeStack->top;
    freeStack->top = freeStack->top->next;
    void* ret = result->data;
    free(result);
    return ret;


}

void freeAll(FreeStack *freeStack) { //delete all nodes at stack and free stack
    while(freeStack->top != NULL) {
        FreeNode * temp = freeStack->top;
        freeStack->top = freeStack->top->next;
        free(temp->data);
        free(temp);
    }

}


void retire(FreeStack *freeStack, void *pointer) { //add Node to the list to be later used
    free_push(freeStack,pointer);
}
void *reuse(FreeStack *freeStack) { //free node
    void *result = free_pop(freeStack);
    return result;
}

 */