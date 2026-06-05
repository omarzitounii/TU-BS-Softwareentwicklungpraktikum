//
// Created by Keanu Wirz on 22.05.23.
//
#include "benchmark.h"
#include "lf_stack.h"


//lock free stack
_Atomic Stack top;

//lock based stack
lb_stack_t stack;

/*lock free queue
 * CMake need to be connected
queue_t lf_queue = initialize(&queue);
 */

/*lock based queue
 * CMake need to be connected
lb_queue_t lb_queue;
 */

//lock free set
/*CMake need to be connected
node_t* head =initialise_node(-5);
node_t* tail=initialise_node(-3);
initialise_list(head, tail);
 */

/*lock based set
 * not implemented yet
lb_set_t lb_set;
 */

//mutex for lock based
pthread_mutex_t mutex;

//thread counter for lf benchmark
int count_lf;

//thread counter for lb benchmark
int count_lb;

//Benchmark functions that are currently used
void lock_free_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion){
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

    FreeStack freeStacks[number_threads];
    // init free stacks
    for(int i = 0; i < number_threads; i++){
        freeStacks[i].top = NULL;
    }


    //create threads depending on the data structure
    bench_args_with_freelist_t* ar[number_threads];
    if (args.structure == STACK) {
        for (int i = 0; i < number_threads; i++) {
            bench_args_with_freelist_t* args_with_list = malloc(sizeof(bench_args_with_freelist_t));
            args_with_list->args = &args;
            args_with_list->freeStack = &freeStacks[i];
            pthread_create(&tid[i], NULL, thread_lock_free_benchmark_stack, (void *) args_with_list);
            //ar[i] = args_with_list;
        }
    }
    if (args.structure == QUEUE) {
        for (int i = 0; i < number_threads; i++) {
            pthread_create(&tid[i], NULL, thread_lock_free_benchmark_queue, (void *) &args);
        }
    }
    if (args.structure == SET) {
        for (int i = 0; i < number_threads; i++) {
            pthread_create(&tid[i], NULL, thread_lock_free_benchmark_set, (void *) &args);
        }
    }
    //join threads
    for(int i = 0;i<number_threads;i++){
        pthread_join(tid[i],NULL);
    }
    for(int i = 0; i < number_threads; i++) {
        if(args.structure == STACK) {
            freeAll(&freeStacks[i]);
            //free (ar[i]);
        }
    }
    FreeStack lastStack;
    lastStack.top = NULL;
    while(true) {
        if(stack_pop(&top, &lastStack) == -1) {
            break;
        }
    }
    freeAll(&lastStack);
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
        printf("overall lock free: %f for %d insertion modifications\n",overall_time[i],i+1);
        printf("latency lock free: %f for %d insertion modifications\n",mod_latency[i],i+1);
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
        printf("overall lock free: %f for %d deletion modifications\n",overall_time[i],i+1);
        printf("latency lock free: %f for %d deletion modifications\n",mod_latency[i],i+1);
    }
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_free_result,"%f,",overall_time[i]);
    }
    fprintf(lock_free_result,"%f\n",overall_time[number_mod-1]);
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_free_result,"%f,",mod_latency[i]);
    }
    fprintf(lock_free_result,"%f\n",mod_latency[number_mod-1]);
}

void lock_based_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion){
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
        printf("overall lock based: %f for %d insertion modifications\n",overall_time[i],i+1);
        printf("latency lock based: %f for %d insertion modifications\n",mod_latency[i],i+1);
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
        printf("overall lock based: %f for %d deletion modifications\n",overall_time[i],i+1);
        printf("latency lock based: %f for %d deletion modifications\n",mod_latency[i],i+1);
    }
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_based_result,"%f,",overall_time[i]);
    }
    fprintf(lock_based_result,"%f\n",overall_time[number_mod-1]);
    for(int i=0;i<number_mod-1;i++){
        fprintf(lock_based_result,"%f,",mod_latency[i]);
    }
    fprintf(lock_based_result,"%f\n",mod_latency[number_mod-1]);
}

//Benchmark that single Thread does for lock free stack data structure
void *  thread_lock_free_benchmark_stack(void *args){
    bench_args_with_freelist_t* benchArgsWithFreelist = (bench_args_with_freelist_t*) args;
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
                //push every even value and measure time
                tic_lat = clock();
                stack_push(&top, j, benchArgsWithFreelist->freeStack);
                toc_lat = clock();
            } else {
                //pop when uneven and measure time
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
    free(args);
    return NULL;
}

//Benchmark that single Thread does for lock based stack data structure
void *thread_lock_based_benchmark_stack(void *args){
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
                //push every even value and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                lb_push(&stack, j);
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            } else {
                //pop when uneven and measure time with locks
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
void *thread_lock_free_benchmark_queue(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
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
                //push every even value and measure time
                tic_lat = clock();
                //enqueue(&lf_queue, j); CMake need to be connected
                toc_lat = clock();
            } else {
                //pop when uneven and measure time
                tic_lat = clock();
                //dequeue(&lf_queue, NULL); CMake need to be connected
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
void *thread_lock_based_benchmark_queue(void *args){
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
                //push every even value and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                //lb_enqueue(&lb_queue, j); CMake need to be connected
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            } else {
                //pop when uneven and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                //lb_dequeue(&lb_queue); CMake need to be connected
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
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*)args;
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
                //push every even value and measure time
                tic_lat = clock();
                //insert(j); CMake need to be connected
                toc_lat = clock();
            }
            else {
                //pop when uneven and measure time
                tic_lat = clock();
                //delete(j); CMake need to be connected
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
            if (i < bench_args->number_insertion) {
                //push every even value and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                //lb_insert(j); CMake need to be connected
                pthread_mutex_unlock(&mutex);
                toc_lat = clock();
            }
            else {
                //pop when uneven and measure time with locks
                tic_lat = clock();
                pthread_mutex_lock(&mutex);
                //lb_delete(j); CMake need to be connected
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


//Functions for testing purposes
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




