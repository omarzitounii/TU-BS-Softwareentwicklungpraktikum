#include <stdio.h>
#include <pthread.h>

#include "harrislist.h"

//Test function removes every even number
void *test1(){
    for(int i = 2; i < 1000; i++) {
        if(insert(i) == 0){
            printf("Number already in Set \n");
        } else {
            printf("thread1: inserted %d \n", i);
        }
        if(i % 2 == 0) {
            delete(i);
            printf("thread1: deleted %d \n", i);
        }
    }
}

//test function removes every third number
void *test2(){
    for(int i = 2; i < 1000; i++) {
        insert(i);
        printf("thread2: inserted %d \n", i);
        if(i % 3 == 0) {
            delete(i);
            printf("thread2: deleted %d \n", i);
        }
    }
}

int main() {
    //create thread with test functions and print list
    node_t* head = initialise_node(-5);
    node_t* t = initialise_node(-3);
    initialise_list(head, t);
    pthread_t thread1, thread2, thread3, thread4;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int t1 = pthread_create(&thread1,&attr,test1,NULL);
    printf("t1: %d\n",t1);
    int t2 = pthread_create(&thread2,&attr,test2,NULL);
    printf("t2: %d\n",t2);
    int t3 = pthread_create(&thread3,&attr,test1,NULL);
    printf("t1: %d\n",t3);
    int t4 = pthread_create(&thread4,&attr,test2,NULL);
    printf("t2: %d\n",t4);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    print_list();
}




