
// Created by Keanu Wirz on 22.05.23.
//
#include <stdlib.h>
#include "../header/benchmark.h"
#include "../header/lf_stack.h"
#include "../header/M_S_Queue.h"



//lock free stack
_Atomic Stack top;

queue_t queue;

list_t lf_list;
harris_node_t* set_head;
harris_node_t* set_tail;

//lock based stack
lb_stack_t stack;

//lock based queue
lb_queue_t lb_queue;

//lock based set
simple_set lb_set;

//mutex for lock based
pthread_mutex_t mutex;

//thread counter for lf benchmark
int count_lf;

//thread counter for lb benchmark
int count_lb;

//used for tests
bool test_ = false;

//Benchmark functions that are currently used
int lock_free_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion){
    //invalid number of threads
    if (number_threads < 1) {
        test_ = true;
        fprintf(stderr, "Error: Number of threads should be greater than 1 \n");
        return -1;
    }
    //invalid number of insertions and deletions
    if (number_insertion == 0 && number_deletion == 0) {
        test_ = true;
        fprintf(stderr,"Error: The sum of number of insertion and deletion should be greater or equal than 1 \n");
        return -1;
    }
    //invalid number of insertions
    if (number_insertion < 0) {
        test_ = true;
        fprintf(stderr,"Error: Number of insertion should be positive \n");
        return -1;
    }
    //invalid number of deletions
    if (number_deletion < 0) {
        test_ = true;
        fprintf(stderr,"Error: Number of deletion should be positive \n");
        return -1;
    }

    //number of modification
    int number_mod = number_insertion + number_deletion;
    //create data arrays
    float overall_thread_time[number_threads][number_mod];
    float overall_time[number_mod];
    float mod_thread_latency[number_threads][number_mod];
    float mod_latency[number_mod];
    //create and fill arguments for single thread benchmark function
    bench_args_t args;
    args.number_insertion = number_insertion;
    args.number_deletion = number_deletion;
    args.latency=(void*)&mod_thread_latency;
    args.overall=(void*)&overall_thread_time;
    args.structure = structure;
    //threads
    pthread_t tid[number_threads];
    //WIP create benchmark.csv with benchmark values to import into java
    FILE *lock_free_result;
    lock_free_result = fopen("lock_free_result.csv","w+");
    //lock free thread counter
    count_lf = 0;

    //init free stacks
    stackFreeStack stack_free_stacks[number_threads];
    msqueueFreeStack msqueue_free_stacks[number_threads];
    harrisFreeStack harris_free_stacks[number_threads];
    if (args.structure == STACK) {
        for (int i = 0; i < number_threads; i++) {
            stack_free_stacks[i].top = NULL;
        }
    }
    if (args.structure == QUEUE) {
        for (int i = 0; i < number_threads; i++) {
            msqueue_free_stacks[i].top = NULL;
        }
    }
    if (args.structure == SET) {
        for (int i = 0; i < number_threads; i++) {
            harris_free_stacks[i].top = NULL;
        }
    }
    bench_args_with_freelist_stack_t* ar_stack[number_threads];
    bench_args_with_freelist_msqueue_t* ar_queue[number_threads];
    bench_args_with_freelist_harris_t* ar_set[number_threads];
    //create threads depending on the data structure
    if (args.structure == STACK) {
        for (int i = 0; i < number_threads; i++) {
            ar_stack[i] = malloc(sizeof(bench_args_with_freelist_stack_t));
            ar_stack[i]->args = &args;
            ar_stack[i]->freeStack = &stack_free_stacks[i];
            pthread_create(&tid[i], NULL, thread_lock_free_benchmark_stack, (void *) ar_stack[i]);
        }
    }
    if (args.structure == QUEUE) {
        initialize(&queue);
        for (int i = 0; i < number_threads; i++) {
            ar_queue[i] = malloc(sizeof(bench_args_with_freelist_msqueue_t));
            ar_queue[i]->args = &args;
            ar_queue[i]->freeStack = &msqueue_free_stacks[i];
            pthread_create(&tid[i], NULL, thread_lock_free_benchmark_queue, (void *) ar_queue[i]);
        }
    }
    if (args.structure == SET) {
        set_head = malloc(sizeof(harris_node_t));
        set_tail = malloc(sizeof(harris_node_t));
        initialise_node(set_head,-3);
        initialise_node(set_tail,-5);
        initialise_list(&lf_list,set_head,set_tail);
        for (int i = 0; i < number_threads; i++) {
            ar_set[i] = malloc(sizeof(bench_args_with_freelist_harris_t));
            ar_set[i]->args = &args;
            ar_set[i]->freeStack = &harris_free_stacks[i];
            pthread_create(&tid[i], NULL, thread_lock_free_benchmark_set, (void *) ar_set[i]);
        }
    }
    //join threads
    for(int i = 0;i<number_threads;i++){
        pthread_join(tid[i],NULL);
    }
    for(int i = 0; i < number_threads; i++) {
        if(args.structure == STACK) {
            stack_freeAll(&stack_free_stacks[i]);
            free(ar_stack[i]);
        }
        if(args.structure == QUEUE) {
            msqueue_freeAll(&msqueue_free_stacks[i]);
            free(ar_queue[i]);
        }
        if(args.structure == SET) {
            harris_freeAll(&harris_free_stacks[i]);
            free(ar_set[i]);
        }


    }


    //calculate avg times between all threads and print them
    for(int i=0;i<number_insertion;i++){
        float sum_overall=0;
        float sum_latency=0;
        for(int j=0;j<number_threads;j++) {
            sum_overall += overall_thread_time[j][i];
            sum_latency += mod_thread_latency[j][i];
        }
        overall_time[i] = sum_overall /(float)number_threads;
        mod_latency[i] = sum_latency /(float)number_threads;
        //printf("overall lock free: %f for %d insertion modifications\n",overall_time[i],i+1);
        //printf("latency lock free: %f for %d insertion modifications\n",mod_latency[i],i+1);
    }
    for(int i=number_insertion;i<number_mod;i++){
        float sum_overall=0;
        float sum_latency=0;
        for(int j=0;j<number_threads;j++) {
            sum_overall += overall_thread_time[j][i];
            sum_latency += mod_thread_latency[j][i];
        }
        overall_time[i] = sum_overall /(float)number_threads;
        mod_latency[i] = sum_latency /(float)number_threads;
        mod_latency[0] = 1;
        //printf("overall lock free: %f for %d deletion modifications\n",overall_time[i],i+1);
        //printf("latency lock free: %f for %d deletion modifications\n",mod_latency[i],i+1);
    }
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_free_result,"%f,",overall_time[i]);
    }
    fprintf(lock_free_result,"%f\n",overall_time[number_mod-1]);
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_free_result,"%f,",mod_latency[i]);
    }
    fprintf(lock_free_result,"%f\n",mod_latency[number_mod-1]);
    return 0;
}

int lock_based_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion){
    //invalid number of threads
    if (number_threads < 1) {
        test_ = true;
        fprintf(stderr, "Error: Number of threads should be greater than 1 \n");
        return -1;
    }
    //invalid number of insertions and deletions
    if (number_insertion == 0 && number_deletion == 0) {
        test_ = true;
        fprintf(stderr,"Error: The sum of number of insertion and deletion should be greater or equal than 1 \n");
        return -1;
    }
    //invalid number of insertions
    if (number_insertion < 0) {
        test_ = true;
        fprintf(stderr,"Error: Number of insertion should be positive \n");
        return -1;
    }
    //invalid number of deletions
    if (number_deletion < 0) {
        test_ = true;
        fprintf(stderr,"Error: Number of deletion should be positive \n");
        return -1;
    }

    //number of modification
    int number_mod = number_insertion + number_deletion;
    //create data arrays
    float overall_thread_time[number_threads][number_mod];
    float overall_time[number_mod];
    float mod_thread_latency[number_threads][number_mod];
    float mod_latency[number_mod];
    //create and fill arguments for single thread benchmark function
    bench_args_t args;
    args.number_insertion = number_insertion;
    args.number_deletion = number_deletion;
    args.latency=(void*)&mod_thread_latency;
    args.overall=(void*)&overall_thread_time;
    args.structure = structure;
    //threads
    pthread_t tid[number_threads];
    //WIP create benchmark.csv with benchmark values to import into java
    FILE *lock_based_result;
    lock_based_result = fopen("lock_based_result.csv","w+");
    //lock based thread counter
    count_lb = 0;
    //create threads depending on the data structure and mutex
    pthread_mutex_init(&mutex,NULL);

    if (args.structure == STACK) {
        for (int i = 0; i < number_threads; i++) {
            pthread_create(&tid[i], NULL, thread_lock_based_benchmark_stack, (void *) &args);
        }
    }
    if (args.structure == QUEUE) {
        for (int i = 0; i < number_threads; i++) {
            pthread_create(&tid[i], NULL, thread_lock_based_benchmark_queue, (void *) &args);
        }
    }
    if (args.structure == SET) {
        set_init(&lb_set);
        for (int i = 0; i < number_threads; i++) {
            pthread_create(&tid[i], NULL, thread_lock_based_benchmark_set, (void *) &args);
        }
    }
    //wait for join
    for(int i = 0;i<number_threads;i++){
        pthread_join(tid[i],NULL);
    }
    //calculate avg times between all threads and print them
    for(int i=0;i<number_insertion;i++){
        float sum_overall=0;
        float sum_latency=0;
        for(int j=0;j<number_threads;j++) {
            sum_overall += overall_thread_time[j][i];
            sum_latency += mod_thread_latency[j][i];
        }
        overall_time[i] = sum_overall /(float)number_threads;
        mod_latency[i] = sum_latency /(float)number_threads;
        //printf("overall lock based: %f for %d insertion modifications\n",overall_time[i],i+1);
        //printf("latency lock based: %f for %d insertion modifications\n",mod_latency[i],i+1);
    }
    for(int i=number_insertion;i<number_mod;i++){
        float sum_overall=0;
        float sum_latency=0;
        for(int j=0;j<number_threads;j++) {
            sum_overall += overall_thread_time[j][i];
            sum_latency += mod_thread_latency[j][i];
        }
        overall_time[i] = sum_overall /(float)number_threads;
        mod_latency[i] = sum_latency /(float)number_threads;
        mod_latency[0] = 1;
        //printf("overall lock based: %f for %d deletion modifications\n",overall_time[i],i+1);
        //printf("latency lock based: %f for %d deletion modifications\n",mod_latency[i],i+1);
    }
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_based_result,"%f,",overall_time[i]);
    }
    fprintf(lock_based_result,"%f\n",overall_time[number_mod-1]);
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_based_result,"%f,",mod_latency[i]);
    }
    fprintf(lock_based_result,"%f\n",mod_latency[number_mod-1]);
    return 0;
}

//Benchmark that single Thread does for lock free stack data structure
void* thread_lock_free_benchmark_stack(void *args){
    bench_args_with_freelist_stack_t* benchArgsWithFreelist = (bench_args_with_freelist_stack_t*) args;
    //cast void pointer to argument pointer
    bench_args_t* bench_args = benchArgsWithFreelist->args;

    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lf,1);
    //number of modification
    int number_mod = bench_args->number_insertion + bench_args->number_deletion;
    //cast arguments into usable arrays
    float (*latency)[k+1][number_mod] = bench_args->latency;
    float (*overall)[k+1][number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i < bench_args->number_insertion) {
                //push value and measure time
                tic_lat = clock();
                stack_push(&top, j, benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            } else {
                //pop and measure time
                tic_lat = clock();
                stack_pop(&top, benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            }
            //fill single thread array with times and calc sum
            single_lat[j] = (float)(toc_lat - tic_lat)/(float)CLOCKS_PER_SEC;
            avg = avg+single_lat[j];
        }
        //calculate average for this thread
        avg = avg / (float)i+1;
        (*latency)[k][i] = avg;
        //stop overall timer
        clock_t toc_overall = clock();
        (*overall)[k][i] = (float)(toc_overall - tic_overall)/(float)CLOCKS_PER_SEC;
    }
    return NULL;
}

//Benchmark that single Thread does for lock based stack data structure
void* thread_lock_based_benchmark_stack(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lb,1);
    //number of modification
    int number_mod = bench_args->number_insertion + bench_args->number_deletion;
    //cast arguments into usable arrays
    float (*latency)[k+1][number_mod] = bench_args->latency;
    float (*overall)[k+1][number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i < bench_args->number_insertion) {
                //push and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                lb_push(&stack, j);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            } else {
                //pop and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                lb_pop(&stack);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            }
            //fill single thread array with times and calc sum
            single_lat[j] = (float)(toc_lat - tic_lat)/(float)CLOCKS_PER_SEC;
            avg = avg+single_lat[j];
        }
        //calculate average for this thread
        avg = avg / (float)i+1;
        (*latency)[k][i] = avg;
        //stop overall timer
        clock_t toc_overall = clock();
        (*overall)[k][i] = (float)(toc_overall - tic_overall)/(float)CLOCKS_PER_SEC;
    }
    return NULL;
}

//Benchmark that single Thread does for lock free queue data structure
void* thread_lock_free_benchmark_queue(void *args){

    bench_args_with_freelist_msqueue_t* benchArgsWithFreelist = (bench_args_with_freelist_msqueue_t*) args;
    //cast void pointer to argument pointer
    bench_args_t* bench_args = benchArgsWithFreelist->args;

    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lf,1);
    //number of modification
    int number_mod = bench_args->number_insertion + bench_args->number_deletion;
    //cast arguments into usable arrays
    float (*latency)[k+1][number_mod] = bench_args->latency;
    float (*overall)[k+1][number_mod] = bench_args->overall;
    //start overall timer
    int value;
    clock_t tic_overall = clock();
    for(int i=0;i<number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i < bench_args->number_insertion) {
                //enqueue and measure time with locks
                tic_lat = clock();
                enqueue(&queue, j,benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            } else {
                //dequeue and measure time with locks
                tic_lat = clock();
                dequeue(&queue, &value, benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            }
            //fill single thread array with times and calc sum
            single_lat[j] = (float)(toc_lat - tic_lat)/(float)CLOCKS_PER_SEC;
            avg = avg+single_lat[j];
        }
        //calculate average for this thread
        avg = avg / (float)i+1;
        (*latency)[k][i] = avg;
        //stop overall timer
        clock_t toc_overall = clock();
        (*overall)[k][i] = (float)(toc_overall - tic_overall)/(float)CLOCKS_PER_SEC;
    }
    return NULL;
}

//Benchmark that single Thread does for lock based queue data structure
void* thread_lock_based_benchmark_queue(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lb,1);
    //number of modification
    int number_mod = bench_args->number_insertion + bench_args->number_deletion;
    //cast arguments into usable arrays
    float (*latency)[k+1][number_mod] = bench_args->latency;
    float (*overall)[k+1][number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i < bench_args->number_insertion) {
                //enqueue and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                lb_enqueue(&lb_queue, j);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            } else {
                //dequeue and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                lb_dequeue(&lb_queue);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            }
            //fill single thread array with times and calc sum
            single_lat[j] = (float)(toc_lat - tic_lat)/(float)CLOCKS_PER_SEC;
            avg = avg+single_lat[j];
        }
        //calculate average for this thread
        avg = avg / (float)i+1;
        (*latency)[k][i] = avg;
        //stop overall timer
        clock_t toc_overall = clock();
        (*overall)[k][i] = (float)(toc_overall - tic_overall)/(float)CLOCKS_PER_SEC;
    }
    return NULL;
}

//Benchmark that single Thread does for lock free set data structure
void* thread_lock_free_benchmark_set(void* args) {
    bench_args_with_freelist_harris_t* benchArgsWithFreelist = (bench_args_with_freelist_harris_t*) args;
    //cast void pointer to argument pointer
    bench_args_t* bench_args = benchArgsWithFreelist->args;

    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*) & count_lf, 1);
    //number of modification
    int number_mod = bench_args->number_insertion + bench_args->number_deletion;
    //cast arguments into usable arrays
    float(*latency)[k + 1][number_mod] = bench_args->latency;
    float(*overall)[k + 1][number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for (int i = 0; i < number_mod; i++) {
        float avg = 0;
        float single_lat[i + 1];
        for (int j = 0; j <= i; j++) {
            clock_t tic_lat, toc_lat;
            if (i < bench_args->number_insertion) {
                //insert and measure time with locks
                tic_lat = clock();
                insert(&lf_list,j,benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            }
            else {
                //delete and measure time with locks
                tic_lat = clock();
                delete(&lf_list,j,benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            }
            //fill single thread array with times and calc sum
            single_lat[j] = (float)(toc_lat - tic_lat) / (float)CLOCKS_PER_SEC;
            avg = avg + single_lat[j];
        }
        //calculate average for this thread
        avg = avg / (float)i + 1;
        (*latency)[k][i] = avg;
        //stop overall timer
        clock_t toc_overall = clock();
        (*overall)[k][i] = (float)(toc_overall - tic_overall) / (float)CLOCKS_PER_SEC;
    }
    harris_freeAll(benchArgsWithFreelist->freeStack);
    return NULL;
}

//Benchmark that single Thread does for lock based set data structure
void* thread_lock_based_benchmark_set(void* args) {
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*)args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*) & count_lb, 1);
    //number of modification
    int number_mod = bench_args->number_insertion + bench_args->number_deletion;
    //cast arguments into usable arrays
    float(*latency)[k + 1][number_mod] = bench_args->latency;
    float(*overall)[k + 1][number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for (int i = 0; i < number_mod; i++) {
        float avg = 0;
        float single_lat[i + 1];
        for (int j = 0; j <= i; j++) {
            clock_t tic_lat, toc_lat;
            if(i < bench_args->number_insertion) {
                //insert and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                set_add(&lb_set,(char*) &j);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            }
            else {
                //delete measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                set_remove(&lb_set, (char*)&j);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            }
            //fill single thread array with times and calc sum
            single_lat[j] = (float)(toc_lat - tic_lat) / (float)CLOCKS_PER_SEC;
            avg = avg + single_lat[j];
        }
        //calculate average for this thread
        avg = avg / (float)i + 1;
        (*latency)[k][i] = avg;
        //stop overall timer
        clock_t toc_overall = clock();
        (*overall)[k][i] = (float)(toc_overall - tic_overall) / (float)CLOCKS_PER_SEC;
    }
    return NULL;
}


//______________________________________________________________________________________________________________________
//old other Variants of Benchmark
/*
void pushBenchmark(int threadCount, long* averagePush, long* totalTime) {
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


    *totalTime = (end_time.tv_sec * 1000000000 + end_time.tv_nsec) - (start_time.tv_sec * 1000000000 + start_time.tv_nsec);
    *averagePush= ((pushend_time.tv_sec * 1000000000 + pushend_time.tv_nsec) - (pushstart_time.tv_sec * 1000000000 + pushstart_time.tv_nsec))/threadCount ;


}

void popBenchmark(int threadCount, long* averagePop, long* totalTime) {
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



    *totalTime += (end_time.tv_sec * 1000000000 + end_time.tv_nsec) - (start_time.tv_sec * 1000000000 + start_time.tv_nsec);
    *averagePop = ((popend_time.tv_sec * 1000000000 + popend_time.tv_nsec) - (popstart_time.tv_sec * 1000000000 + popstart_time.tv_nsec))/threadCount;




    //*totalTime +=  ((double)(totaltime_end - totaltime_start)) / CLOCKS_PER_SEC;
}
*/

//Functions for testing purposes

/*kommentiert weill alle muss angepasst nach dem (satck/msqueue/harris)FreeStack
void *test1(FreeStack *freeStack){
    for(int i=0;i<100000000;i++){
        stack_push(&top,i, freeStack);
    }
    return NULL;
}

void *test2(FreeStack *freeStack){
    for(int i=0;i<50000000;i++){
        stack_pop(&top, freeStack);
    }
    return NULL;
}

void *test3(FreeStack *freeStack){
    for(int i=0;i<80000000;i++){
        stack_push(&top,i, freeStack);
        if(i%2==0){
            stack_pop(&top, freeStack);
        }

    }
    return NULL;
}

void *test4(FreeStack *freeStack){
    for(int i=0;i<120000000;i++){
        stack_push(&top,i, freeStack);
        if(i%5==0){
            stack_pop(&top, freeStack);
        }
    }
    return NULL;
}

//push and pop variations that the old Benchmark uses
void *push(void *input)
{
    FreeStack freeStack;
    for(int i=0; i<100000; i++)
    {
        stack_push(&top, i, (FreeStack *) &freeStack);
        //printf("push %d\n",i);
    }

    pthread_exit(NULL);

}

void *pop(void *input)
{   FreeStack freeStack;
    for(int i=0; i<100000;)
    {
        int result;
        result = stack_pop(&top, (FreeStack *) &freeStack);
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

*/

/*
 * Tests für benchmark
 */

//lock free

//[test]
void test_non_positive_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, -5, 10, 10);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//[test]
void test_null_insertion_deletion_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 0, 0);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//[test]
void test_negative_insertions_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, -5, 10);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//[test]
void test_negative_deletions_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 10, -5);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//--------------------------
//[test]
void test_nonzero_insertions_zero_deletions_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 10, 0);
        if (test_ == true) {
            printf("test failed \n");
        } else {
            printf("test succeed \n");
        }
    }
}

//[test]
void test_zero_insertions_nonzero_deletions_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 0, 10);
        if (test_ == true) {
            printf("test failed \n");
        } else {
            printf("test succeed \n");
        }
    }
}

//[test]
void test_nonzero_insertions_nonzero_deletions_thread_lock_free_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 10, 10);
        if (test_ == true) {
            printf("test failed \n");
        } else {
            printf("test succeed \n");
        }
    }
}

//---------
//lock based
//---------

//[test]
void test_non_positive_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, -5, 10, 10);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//[test]
void test_null_insertion_deletion_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 0, 0);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//[test]
void test_negative_insertions_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, -5, 10);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//[test]
void test_negative_deletions_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 10, -5);
        if (test_ == true) {
            printf("test succeed \n");
        } else {
            printf("test failed \n");
        }
    }
}

//--------------------------
//[test]
void test_nonzero_insertions_zero_deletions_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 10, 0);
        if (test_ == true) {
            printf("test failed \n");
        } else {
            printf("test succeed \n");
        }
    }
}

//[test]
void test_zero_insertions_nonzero_deletions_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 0, 10);
        if (test_ == true) {
            printf("test failed \n");
        } else {
            printf("test succeed \n");
        }
    }
}

//[test]
void test_nonzero_insertions_nonzero_deletions_thread_lock_based_benchmark(){
    for (int i=0;i<3;i++) {
        lock_free_benchmark(i, 2, 10, 10);
        if (test_ == true) {
            printf("test failed \n");
        } else {
            printf("test succeed \n");
        }
    }
}


