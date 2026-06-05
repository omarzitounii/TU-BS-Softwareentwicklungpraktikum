/*
#include <stdio.h>
//#include <pthread.h>
#include <Windows.h>
#include "harrislist.h"



HarrisFreeStack freeStacks[2];

//Test function removes every even number
DWORD WINAPI test1() {
    for(int i = 0; i < 100000; i++) {
        if (insert(i,&freeStacks[0]) == 0) {
            printf("Number already in Set \n");
        } else {
            printf("thread1: inserted %d \n", i);
        }
        if (i % 2 == 0) {
            delete(i,&freeStacks[0]);
            printf("thread1: deleted %d \n", i);
        }
    }
}

//test function removes every third number
DWORD WINAPI test2(){
    for(int i = 0; i < 100000; i++) {
        insert(i,&freeStacks[1]);
        printf("thread2: inserted %d \n", i);
        if(i % 3 == 0) {
            delete(i,&freeStacks[1]);
            printf("thread2: deleted %d \n", i);
        }
    }
    return 0;
}

int main() {
    //create thread with test functions and print list
    harris_node_t* head = malloc(sizeof(harris_node_t));
    initialise_node(head,-5);
    harris_node_t* t = malloc(sizeof(harris_node_t));
    initialise_node(t,-3);
    initialise_list(head, t);
    //Ich weiß leider nicht, wie ich das umschreiben soll, aber es funktioniert auch ohne
    //pthread_attr_t attr;
    //pthread_attr_init(&attr);


    HANDLE thread1, thread2, thread3, thread4;
    thread1 = CreateThread(NULL, 0, test1, 0, 0, NULL);
    int t1 = (int) thread1;
    printf("t1: %d\n",t1);
    thread2 = CreateThread(NULL, 0, test2, 0, 0, NULL);
    int t2 = (int) thread2;
    printf("t2: %d\n",t2);
    thread3 = CreateThread(NULL, 0, test1, 0, 0, NULL);
    int t3 = (int) thread3;
    printf("t1: %d\n",t3);
    thread4 = CreateThread(NULL, 0, test1, 0, 0, NULL);
    int t4 = (int) thread4;
    printf("t2: %d\n",t4);
    WaitForSingleObject (thread1, INFINITE);
    WaitForSingleObject (thread2, INFINITE);
    WaitForSingleObject (thread3, INFINITE);
    WaitForSingleObject (thread4, INFINITE);
    print_list();

}

*/


