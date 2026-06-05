#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <pthread.h>
#include <Windows.h>
#include "M_S_Queue.h"

DWORD WINAPI test1(void* queue) {
    for(int i = 0; i < 1000; ++i) {
        enqueue(queue, i);
        printf("enqueue: %d \n",i);
    }
    return 0;
}

DWORD WINAPI test2(void* queue) {
    int numDequeued = 0;
    while (numDequeued < 1000) {
        int value;
        if (dequeue(queue, &value)) {
            ++numDequeued;
        }
        printf("dequeue\n");
    }
    return 0;
}

int main() {
    queue_t queue;
    initialize(&queue);
    HANDLE thread1, thread2, thread3, thread4;

    /*thread1 = CreateThread(NULL, 0, test1, &queue, 0, NULL);
    thread2 = CreateThread(NULL, 0, test2, &queue, 0, NULL);*/
    thread1 = CreateThread(NULL, 0, test1, &queue, 0, NULL);
    thread2 = CreateThread(NULL, 0, test2, &queue, 0, NULL);
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);
    CloseHandle(thread1);
    CloseHandle(thread2);


    int value;
    //enqueue(&queue, 3);
    if (dequeue(&queue, &value)) {
        printf("Element was deleted\n");
    } else {
        printf("No Element to delete\n");
    }

    return 0;
}
