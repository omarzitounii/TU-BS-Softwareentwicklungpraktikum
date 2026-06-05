#include <stdio.h>
#include <pthread.h>
#include "M_S_Queue.h"

void* test1(void *args) {
    args_with_free_list_t* argsWithFreeList = (args_with_free_list_t*) args;
    FreeStack* freeStack = argsWithFreeList->freeStack;
    queue_t* queue = argsWithFreeList->queue;
    for(int i = 0; i < 10000; ++i) {
        int value;
        //dequeue(queue, &value,freeStack);
        printf("dequeue \n");
        enqueue(queue, i,freeStack);
        printf("enqueue: %d \n",i);
        //dequeue(queue, &value,freeStack);
        printf("dequeue \n");
        enqueue(queue, i,freeStack);
        enqueue(queue, i,freeStack);
    }
    return NULL;
}

void* test2(void* args) {
    args_with_free_list_t* argsWithFreeList = (args_with_free_list_t*) args;
    FreeStack* freeStack = argsWithFreeList->freeStack;
    queue_t* queue = argsWithFreeList->queue;

    for(int i = 0; i < 10000; ++i) {
        int value;
        enqueue(queue, i,freeStack);
        printf("enqueue double: %d \n", i);
        dequeue(queue, &value,freeStack);
        printf("dequeue \n");
        enqueue(queue, i,freeStack);
        printf("enqueue double: %d \n", i);
        //dequeue(queue, &value,freeStack);
    }
    return NULL;
}

int main() {
    /*
    queue_t queue;
    //initialize(&queue);
    //pthread_t thread1, thread2, thread3, thread4;
    FreeStack freeStack, freeStack1;
    freeStack.top = NULL;
    freeStack1.top = NULL;
    initialize(&queue, &freeStack);
    pthread_t thread1, thread2;
    args_with_free_list_t *argsWithFreeList = malloc(sizeof(args_with_free_list_t));
    args_with_free_list_t *argsWithFreeList1 = malloc(sizeof(args_with_free_list_t));
    argsWithFreeList->freeStack = &freeStack;
    argsWithFreeList->queue = &queue;
    argsWithFreeList1->freeStack = &freeStack1;
    argsWithFreeList1->queue = &queue;
    pthread_create(&thread1, NULL, test2, (void *) argsWithFreeList);
    //pthread_create(&thread2, NULL, test2, (void *) argsWithFreeList1);
    pthread_join(thread1, NULL);
    //pthread_join(thread2, NULL);
    freeAll(&freeStack);
    freeAll(&freeStack1);
    free(argsWithFreeList);
    free(argsWithFreeList1);
     */
    queue_t queue;
    initialize(&queue);
    FreeStack freeStacks[4];
    pthread_t tid[4];
    args_with_free_list_t* args[6];
    for(int i = 0; i <4; i++){
        freeStacks[i].top = NULL;
    }
    for (int i = 0; i < 2; i++) {
        args_with_free_list_t *argsWithFreeList = malloc(sizeof(args_with_free_list_t));

        argsWithFreeList->freeStack = &freeStacks[i];
        argsWithFreeList->queue = &queue;
        pthread_create(&tid[i], NULL, test1, (void *) argsWithFreeList);
        args[i] = argsWithFreeList;
    }

    for (int i = 2; i < 4; i++) {
        args_with_free_list_t *argsWithFreeList = malloc(sizeof(args_with_free_list_t));

        argsWithFreeList->freeStack = &freeStacks[i];
        argsWithFreeList->queue = &queue;
        pthread_create(&tid[i], NULL, test2, (void *) argsWithFreeList);
        args[i] = argsWithFreeList;
    }

    for(int i=0; i<4; i++) {
        pthread_join(tid[i], NULL);

    }

    for(int i=0; i<4; i++) {
        freeAll(&freeStacks[i]);
        free(args[i]);
    }
    FreeStack lastStack;
    lastStack.top = NULL;
    while(true) {
        int value;
        if(!dequeue(&queue, &value, &lastStack)) {
            break;
        }
    }
    freeAll(&lastStack);
    return 0;
}


   /* pthread_create(&thread1, NULL, test1, (void *) argsWithFreeList);
    pthread_create(&thread2, NULL, test2,(void *) argsWithFreeList);
    pthread_create(&thread3, NULL, test1, (void *) argsWithFreeList);
    pthread_create(&thread4, NULL, test2, (void *) argsWithFreeList);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    freeAll(&f1);
    freeAll(&f2);
    freeAll(&f3);
    freeAll(&f4);





    int value;
    //enqueue(&queue, 3);
    if (dequeue(&queue, &value, &freeStack)) {
        printf("Element was deleted\n");
    } else {
        printf("No Element to delete\n");
    }
*/


