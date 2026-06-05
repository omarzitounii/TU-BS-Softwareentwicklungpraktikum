//
// Created by Omar and Mohamed Amine on 12.06.2023.
//

#ifndef LIBRARY_LB_QUEUE_H
#define LIBRARY_LB_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Lock Based variant of queue data structure

//The node with value and next
struct node{
    int val;
    struct node *next;
};

//the queue itself with only the head pointer
typedef struct lb_stack{
    struct node* head;
}lb_queue_t;


//enqueue element to the end of the list
void lb_enqueue(lb_queue_t* queue, int value);

//dequeue element from the head of the list
bool lb_dequeue(lb_queue_t* queue);



#endif //LIBRARY_LB_QUEUE_H
