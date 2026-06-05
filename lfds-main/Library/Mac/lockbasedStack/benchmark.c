#include "benchmark.h"
Stack stack;
void *push(void *input)
{
    for(int i=0; i<100000; i++)
    {
        stack_push(&stack, i);
        //printf("push %d\n",i);
    }
    pthread_exit(NULL);
}

void *pop(void *input)
{
    for(int i=0; i<100000;)
    {
        int result;
        result = stack_pop(&stack);
        if(result == -1)
            //printf("the stack is empty\n");
            break;
        else
        {
            i++;
        }

    }
    pthread_exit(NULL);
}
void pushBenchmark(int threadCount, long* averagePush, long* totalTime) {
    pthread_mutex_init(&stack.lock,NULL);
    struct timespec start_time, end_time, pushstart_time, pushend_time;

    pthread_t tid[threadCount*2];
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    clock_gettime(CLOCK_MONOTONIC, &pushstart_time);  //start clock for push timer
    for(int i=0; i<threadCount; i++) //create push threads
        pthread_create(&tid[i],NULL,push,NULL);
    for(int i=threadCount; i<threadCount*2 ; i++) //create pop threads
        pthread_create(&tid[i],NULL,pop,NULL);
    for(int i=0; i<threadCount; i++) //join push threads
        pthread_join(tid[i],NULL);
    clock_gettime(CLOCK_MONOTONIC, &pushend_time);
    clock_t push_end = clock(); // stop push timer
    for(int i=threadCount; i<threadCount*2; i++)
        pthread_join(tid[i],NULL); //join pop threads
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    pthread_mutex_destroy(&stack.lock);


    *totalTime = (end_time.tv_sec * 1000000000 + end_time.tv_nsec) - (start_time.tv_sec * 1000000000 + start_time.tv_nsec);
    *averagePush= ((pushend_time.tv_sec * 1000000000 + pushend_time.tv_nsec) - (pushstart_time.tv_sec * 1000000000 + pushstart_time.tv_nsec))/threadCount ;


}

void popBenchmark(int threadCount, long* averagePop, long* totalTime) {
    pthread_mutex_init(&stack.lock,NULL);
    struct timespec start_time, end_time, popstart_time, popend_time;
    pthread_t tid[threadCount*2];


    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for(int i=0; i<threadCount; i++) //create push threads
        pthread_create(&tid[i],NULL,push,NULL);
    clock_gettime(CLOCK_MONOTONIC, &popstart_time); //start timer for pop

    for(int i=threadCount; i<threadCount*2 ; i++) //create pop threads
        pthread_create(&tid[i],NULL,pop,NULL);
    for(int i=threadCount; i<threadCount*2; i++) //join pop threads
        pthread_join(tid[i],NULL);
    clock_gettime(CLOCK_MONOTONIC, &popend_time);// stop timer for pop
    for(int i=0; i<threadCount; i++) //join push threads
        pthread_join(tid[i],NULL);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    pthread_mutex_destroy(&stack.lock);



    *totalTime += (end_time.tv_sec * 1000000000 + end_time.tv_nsec) - (start_time.tv_sec * 1000000000 + start_time.tv_nsec);
    *averagePop = ((popend_time.tv_sec * 1000000000 + popend_time.tv_nsec) - (popstart_time.tv_sec * 1000000000 + popstart_time.tv_nsec))/threadCount;




    //*totalTime +=  ((double)(totaltime_end - totaltime_start)) / CLOCKS_PER_SEC;
}

