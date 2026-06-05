/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <pthread.h>
#include <Windows.h>
#include "M_S_Queue.h"


/*
void* test1(void* queue) {
    for(int i = 0; i < 1000; ++i) {
        enqueue(queue, i);
        printf("enqueue: %d \n",i);
    }
    return NULL;
}

void* test2(void* queue) {
    for(int i = 0; i < 1000; ++i) {
        int value;
        enqueue(queue, i);
        dequeue(queue, &value);
        enqueue(queue, i);
        printf("dequeue\n");
    }
    return NULL;
}

int main() {
    queue_t queue;
    initialize(&queue);
    pthread_t thread1, thread2, thread3, thread4;

    pthread_create(&thread1, NULL, test1, &queue);
    pthread_create(&thread2, NULL, test2, &queue);
    pthread_create(&thread1, NULL, test1, &queue);
    pthread_create(&thread2, NULL, test2, &queue);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);



    int value;
    //enqueue(&queue, 3);
    if (dequeue(&queue, &value)) {
        printf("Element was deleted\n");
    } else {
        printf("No Element to delete\n");
    }

    return 0;
}
 */



