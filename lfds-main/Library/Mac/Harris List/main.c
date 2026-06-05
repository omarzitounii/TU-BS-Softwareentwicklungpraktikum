#include <stdio.h>
#include <pthread.h>

#include "harrislist.h"

#define THREAD_COUNT 4

//Test function removes every even number
void *test1(void* args){
    args_with_free_list_t* argsWithFreeList = (args_with_free_list_t*) args;
    FreeStack* freeStack = argsWithFreeList->freeStack;
    list_t* set= argsWithFreeList->set;


    for(int i = 0; i < 10000; i++) {
        if(!insert(set,i,freeStack)){
            printf("Number already in Set \n");
        } else {
            printf("thread1: inserted %d \n", i);
        }
        if(i % 2 == 0) {
            delete(set,i,freeStack);
            printf("thread1: deleted %d \n", i);
        }
    }
}

//test function removes every third number
void *test2(void*args){
    args_with_free_list_t* argsWithFreeList = (args_with_free_list_t*) args;
    FreeStack* freeStack = argsWithFreeList->freeStack;
    list_t* set= argsWithFreeList->set;


    for(int i = 0; i < 10000; i++) {
        if(i < 5000) {
            insert(set, i, freeStack);
            printf("thread2: inserted %d \n", i);
        }
        else {
            delete(set,i,freeStack);
            printf("thread2: deleted %d \n", i);
        }
    }
}


int main() {
    //create thread with test functions and print list
    list_t set;
    node_t* head = malloc(sizeof(node_t));
    initialise_node(head,-5);
    node_t* t = malloc(sizeof(node_t));
    initialise_node(t,10);
    initialise_list(&set,head, t);
    pthread_t thread1, thread2, thread3, thread4;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    FreeStack freeStacks[4];
    pthread_t tid[4];
    args_with_free_list_t* args[6];


    for(int i = 0; i < THREAD_COUNT; i++){
        freeStacks[i].top = NULL;
    }
    for(int i=0; i< THREAD_COUNT; i++) {
        args_with_free_list_t *argsWithFreeList = malloc(sizeof(args_with_free_list_t));
        argsWithFreeList->freeStack = &freeStacks[i];
        argsWithFreeList->set= &set;
        int j = pthread_create(&tid[i],&attr,test2,(void *) argsWithFreeList);
        args[i] = argsWithFreeList;
        printf("t: %d\n",j);
    }
    for(int i=0; i< THREAD_COUNT; i++) {
        pthread_join(tid[i], NULL);
    }
    print_list(&set);
    for(int i=0; i < THREAD_COUNT; i++) {
        freeAll(&freeStacks[i]);
        free(args[i]);
    }
    //freeSet(&set);

}





