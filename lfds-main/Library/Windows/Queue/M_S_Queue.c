#include "M_S_Queue.h"

void initialize(queue_t* Q) {
    node_t* node = malloc(sizeof(node_t)); // Allocate a free node
    node->next.ptr = NULL; // Make it the only node in the linked list
    pointer_t node_count = {node, 0};
    // Both Head and Tail point to it
    atomic_store(&Q->Head, node_count); // Create atomic object with a value
    atomic_store(&Q->Tail, node_count);
}

node_t* initialise_node(data_type value) {
    node_t* node = malloc(sizeof(node_t)); // Allocate a new node from the free list
    node->value = value; // Copy enqueued value into node
    node->next.ptr = NULL; // Set next pointer of node to NULL
    return node;
}

pointer_t initialise_pointer() {
    pointer_t pointer;
    pointer.ptr = NULL;
    pointer.count = 0;
    return pointer;
}

void enqueue(queue_t* Q, data_type value) {
    node_t* node = initialise_node(value); // Allocate a new node from the free list
    pointer_t tail = initialise_pointer();
    while (true) { // Keep trying until Enqueue is done
        tail = atomic_load(&(Q->Tail)); // Read Tail.ptr and Tail.count together
        pointer_t next = tail.ptr->next; // Read next ptr and count fields together
        if (atomic_compare_exchange_strong(&Q->Tail, &tail, tail)) { // Are tail and next consistent?
            if (next.ptr == NULL) { // Was Tail pointing to the last node?
                pointer_t node_count = {node, next.count+1};
                if (tail.ptr->next.ptr == next.ptr) { // Try to link node at the end of the linked list
                    tail.ptr->next = node_count;
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


bool dequeue(queue_t *Q, data_type* pointer_value) {
    pointer_t head;
    pointer_t tail;
    while (1) {
        head = atomic_load(&Q->Head);
        tail = atomic_load(&Q->Tail);
        pointer_t next = head.ptr->next;
        //__c11_atomic_compare_exchange_strong(&Q->Head, &head, head, 5, 5)
        if (atomic_compare_exchange_strong(&Q->Head, &head, head)) {
            if (head.ptr == tail.ptr) {
                if (next.ptr == NULL) {
                    return false; // Queue is empty, couldn't dequeue
                }
                pointer_t next_count = {next.ptr, tail.count+1};
                atomic_compare_exchange_strong(&Q->Tail, &tail, next_count);
            } else {
                if (next.ptr == NULL) {
                    return false; // Queue is empty, couldn't dequeue
                }
                *pointer_value = next.ptr->value;
                pointer_t next_counter = {next.ptr, head.count+1};
                if (atomic_compare_exchange_strong(&Q->Head, &head, next_counter)) {
                    break; // Dequeue is done. Exit loop
                }
            }
        }
    }
    free(head.ptr); // It is safe now to free the old dummy node
    return true; // Queue was not empty, dequeue succeeded
}
