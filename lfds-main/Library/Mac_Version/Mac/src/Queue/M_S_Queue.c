#include "../header/M_S_Queue.h"

void initialize(queue_t* Q) {
    ms_queue_node_t* node = malloc(sizeof(ms_queue_node_t)); // Allocate a free node
    pointer_t tmp;
    tmp.ptr = NULL;
    atomic_store(&node->next,tmp); // Make it the only node in the linked list
    pointer_t node_count = {node, 0};
    // Both Head and Tail point to it
    atomic_store(&Q->Head, node_count); // Create atomic object with a value
    atomic_store(&Q->Tail, atomic_load(&Q->Head));
}

ms_queue_node_t* initialise_node_msqueue(data_type value) {
    ms_queue_node_t* node = malloc(sizeof(ms_queue_node_t)); // Allocate a new node from the free list
    node->value = value; // Copy enqueued value into node
    //node->next.ptr = NULL; // Set next pointer of node to NULL
    return node;
}

pointer_t initialise_pointer() {
    pointer_t pointer;
    pointer.ptr = NULL;
    pointer.count = 0;
    return pointer;
}

void enqueue(queue_t* Q, data_type value, struct msqueue_FreeStack *freeStack) {
    ms_queue_node_t * node = msqueue_reuse(freeStack); // Allocate a new node from the free lists
    node->value = value; // Copy enqueued value into node
    pointer_t tmp;
    tmp.ptr = NULL;
    atomic_store(&node->next,tmp); // Set next pointer of node to NULL

    pointer_t tail = initialise_pointer();
    while (true) { // Keep trying until Enqueue is done
        tail = atomic_load(&Q->Tail); // Read Tail.ptr and Tail.count together
        pointer_t next;
        next = tail.ptr->next;
        //if (tail.ptr != NULL) next = tail.ptr->next; // Read next ptr and count fields together
        //else continue;
        if (atomic_compare_exchange_strong(&Q->Tail, &tail, tail)) { // Are tail and next consistent?
            if (next.ptr == NULL) { // Was Tail pointing to the last node?
                pointer_t node_count = {node, next.count+1};
                if (atomic_compare_exchange_strong( &tail.ptr->next, &next, node_count)){ // Try to link node at the end of the linked list
                    break; // Enqueue is done. Exit loop
                }
            } else { // Tail was not pointing to the last node
                pointer_t next_count = {next.ptr, tail.count+1};
                atomic_compare_exchange_weak(&Q->Tail, &tail, next_count); // Try to swing Tail to the next node
            }
        }
    }
    pointer_t node_count = {node, tail.count+1};
    atomic_compare_exchange_weak(&Q->Tail, &tail, node_count); // Try to swing Tail to the next node
}


bool dequeue(queue_t *Q, data_type* pointer_value, struct msqueue_FreeStack* freeStack) {
    pointer_t head;
    while (1) {
        head = atomic_load(&Q->Head);
        pointer_t tail = atomic_load(&Q->Tail);
        //if (head.ptr == tail.ptr) return false;
        //pointer_t next = initialise_pointer();
        pointer_t next;
        next = head.ptr->next;
        //if (head.ptr != NULL) next.ptr = head.ptr->next.ptr;
        //else return false;
        if (atomic_compare_exchange_strong(&Q->Head,&head,head)) {
            if (head.ptr == tail.ptr) {
                if (next.ptr == NULL) {
                    return false; // Queue is empty, couldn't dequeue
                }
                pointer_t next_count = {next.ptr, tail.count+1};
                atomic_compare_exchange_strong(&Q->Tail, &tail, next_count);
            } else {
                if (next.ptr != NULL)
                    *pointer_value = next.ptr->value;
                else return false;
                pointer_t next_counter = {next.ptr, head.count+1};
                if (atomic_compare_exchange_strong(&Q->Head, &head, next_counter)) {
                    break; // Dequeue is done. Exit loop
                }
            }
        }
    }
    msqueue_retire(freeStack, head.ptr);
    //free(head.ptr); // It is safe now to free the old dummy node
    return true; // Queue was not empty, dequeue succeeded
}

//--------------------------------------3 Versionen von diesen FREE STRUCT UND FUNKTIONEN-----------------------


void msqueue_free_push(msqueueFreeStack *freeStack, ms_queue_node_t *data) {//push pointer  to the free list
    msqueueFreeNode* node = malloc(sizeof(msqueueFreeNode));
    node->data = data;
    node->next = freeStack->top;
    freeStack->top = node;
}

ms_queue_node_t *msqueue_free_pop(msqueueFreeStack *freeStack) { //take a adress from the free list
    if(freeStack->top == NULL) { //check if list empty
        return malloc(sizeof(ms_queue_node_t));
    }
    msqueueFreeNode *result = freeStack->top;
    freeStack->top = freeStack->top->next;
    ms_queue_node_t* ret = result->data;
    free(result);
    return ret;


}

void msqueue_freeAll(msqueueFreeStack *freeStack) { //delete all nodes at stack and free stack
    while(freeStack->top != NULL) {
        msqueueFreeNode * temp = freeStack->top;
        freeStack->top = freeStack->top->next;
        free(temp->data);
        free(temp);
    }

}

void msqueue_retire(msqueueFreeStack *freeStack, ms_queue_node_t *pointer) { //add Node to the list to be later used
    msqueue_free_push(freeStack,pointer);
}

ms_queue_node_t *msqueue_reuse(msqueueFreeStack *freeStack) { //free node
    ms_queue_node_t *result = msqueue_free_pop(freeStack);
    return result;
}


void freeQueue(queue_t *Q) {

    while(true) {
        pointer_t head = atomic_load(&Q->Head);
        ms_queue_node_t *temp = head.ptr;
        Q->Head = head.ptr->next;
        free(temp);
        if(head.ptr == NULL) {
            break;
        }

    }
}











