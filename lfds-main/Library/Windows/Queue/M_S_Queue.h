#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#ifndef M_S_QUEUE_M_S_QUEUE_H
#define M_S_QUEUE_M_S_QUEUE_H

typedef struct {
    struct node_t * ptr;
    int count;
} pointer_t;

typedef int data_type;
typedef struct node_t {
    data_type value;
    pointer_t next;
} node_t;

typedef struct {
    _Atomic pointer_t Head;
    _Atomic pointer_t Tail;
} queue_t;

void initialize(queue_t* Q);
void enqueue(queue_t* Q, data_type value);
bool dequeue(queue_t *Q, data_type* pointer_value);


#endif //M_S_QUEUE_M_S_QUEUE_H