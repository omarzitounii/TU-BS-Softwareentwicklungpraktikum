//
// Created by Omar and Mohamed Amine on 12.06.2023.
//

#include <stdlib.h>
#include <stdio.h>
#include "lb_queue.h"
#include <errno.h>

//implementation of lock based queue

void lb_enqueue(lb_queue_t* queue, int value) {
    struct node *insertedElement;
    struct node *helper;
    insertedElement = (struct node*)malloc(sizeof(struct node));
    if (insertedElement == NULL) {
        perror("Error when trying to malloc the memory");
        return;
    }
    insertedElement->val = value;
    insertedElement->next = NULL;
    if (queue->head == NULL) {
        queue->head = insertedElement;
        return;
    }
    else {
        helper = queue->head;
        while(helper->next != NULL) {
            helper = helper->next;
        }
        helper->next = insertedElement;
        return;
    }
}

bool lb_dequeue(lb_queue_t* queue) {
    if (queue->head == NULL) {
        return false;
    }
    struct node* helper;
    helper = queue->head->next;
    free(queue->head);
    queue->head = helper;
    return true;
}





