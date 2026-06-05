//
// Created by Omar and Mohamed Amine on 12.06.2023.
//

#include <stdlib.h>
#include <stdio.h>
#include "../header/lb_queue.h"
#include <errno.h>

//implementation of lock based queue

void lb_enqueue(lb_queue_t* queue, int value) {
    struct lb_queue_node *insertedElement;
    struct lb_queue_node *helper;
    insertedElement = (struct lb_queue_node*)malloc(sizeof(struct lb_queue_node));
    if (insertedElement == NULL) {
        perror("Error when trying to malloc the memory");
        return;
    }
    insertedElement->val = value;
    insertedElement->next = NULL;
    if (queue->head == NULL) {
        queue->head = insertedElement;
        queue->tail = insertedElement;
        return;
    }
    else {
        queue->tail->next = insertedElement;
        queue->tail = insertedElement;
        return;
    }
}

bool lb_dequeue(lb_queue_t* queue) {
    if (queue->head == NULL) {
        return false;
    }
    struct lb_queue_node* helper;
    helper = queue->head->next;
    free(queue->head);
    queue->head = helper;
    return true;
}
