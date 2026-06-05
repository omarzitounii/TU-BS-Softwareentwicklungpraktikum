//
// Created by Mohamed Amine, Omar & Keanu on 12.06.23.
//

#define _CRT_SECURE_NO_WARNINGS
#include "benchmark.h"




//lock free stack
_Atomic Stack top;

//lock based stack
lb_stack_t stack;

/*lock free queue
 * CMake need to be connected
queue_t lf_queue = initialize(&queue);
 */

/*lock based queue
 * not implemented yet
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
CRITICAL_SECTION mutex;

//thread counter for lf benchmark
int count_lf;

//thread counter for lb benchmark
int count_lb;

//Benchmark functions that are currently used
void lock_free_benchmark(enum data_structure structure, int number_threads, int number_mod){
    //create data arrays
    float overall_thread_time[number_threads][number_mod];
    float overall_time[number_mod];
    float mod_thread_latency[number_threads][number_mod];
    float mod_latency[number_mod];
    //create and fill arguments for single thread benchmark function
    bench_args_t args;
    args.number_mod=number_mod;
    args.latency=(void*)&mod_thread_latency;
    args.overall=(void*)&overall_thread_time;
    args.structure = structure;
    //threads
    HANDLE tid[number_threads];
    //WIP create bencmark.csv with benchmark values to import into java
    FILE* lock_free_result;
    errno_t err;

    err = fopen_s(&lock_free_result, "lock_free_result.csv", "w+");
    if (err != 0) {
    // Handle error opening the file
    } else {
    // File opened successfully
    }
    //lock free thread counter
    count_lf = 0;
    //create threads depending on the data structure
    if(args.structure == STACK) {
        for (int i = 0; i < number_threads; i++) {
            HANDLE tid[i];
            tid[i] = CreateThread(NULL, 0, thread_lock_free_benchmark_stack, &args, 0, NULL);
        }
    }
    if(args.structure == QUEUE) {
        for (int i = 0; i < number_threads; i++) {
            HANDLE tid[i];
            tid[i] = CreateThread(NULL, 0, thread_lock_free_benchmark_queue, &args, 0, NULL);
        }
    }
    if(args.structure == SET) {
        for (int i = 0; i < number_threads; i++) {
            HANDLE tid[i];
            tid[i] = CreateThread(NULL, 0, thread_lock_free_benchmark_set, &args, 0, NULL);
        }
    }
    //join threads
    for(int i = 0;i<number_threads;i++){
        WaitForSingleObject (tid[i],INFINITE);
    }
    //calculate avg times between all threads and print them
    for(int i=0;i<number_mod;i++){
        float sum_overall=0;
        float sum_latency=0;
        for(int j=0;j<number_threads;j++) {
            sum_overall += overall_thread_time[j][i];
            sum_latency += mod_thread_latency[j][i];
        }
        overall_time[i] = sum_overall /(float)number_threads;
        mod_latency[i] = sum_latency /(float)number_threads;
        printf("overall lock free: %f for %d modifications\n",overall_time[i],i+1);
        printf("latency lock free: %f for %d modifications\n",mod_latency[i],i+1);
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

void lock_based_benchmark(enum data_structure structure, int number_threads, int number_mod){
    //create data arrays
    float overall_thread_time[number_threads][number_mod];
    float overall_time[number_mod];
    float mod_thread_latency[number_threads][number_mod];
    float mod_latency[number_mod];
    //create and fill arguments for single thread benchmark function
    bench_args_t args;
    args.number_mod=number_mod;
    args.latency=(void*)&mod_thread_latency;
    args.overall=(void*)&overall_thread_time;
    args.structure = structure;
    //threads
    HANDLE tid[number_threads];
    WaitForSingleObject (tid[number_threads], INFINITE);
    //WIP create bencmark.csv with benchmark values to import into java
    FILE *lock_based_result;
    lock_based_result = fopen("lock_based_result.csv","w+");
    //lock based thread counter
    count_lb = 0;
    //create mutex
    InitializeCriticalSection(&mutex);
    //create threads depending on the data structure
    if(args.structure == STACK) {
        for (int i = 0; i < number_threads; i++) {
            HANDLE tid[i];
            tid[i] = CreateThread(NULL, 0, thread_lock_based_benchmark_stack, &args, 0, NULL);
        }
    }
    if(args.structure == QUEUE) {
        for (int i = 0; i < number_threads; i++) {
            HANDLE tid[i];
            tid[i] = CreateThread(NULL, 0, thread_lock_based_benchmark_queue, &args, 0, NULL);
        }
    }
    if(args.structure == SET) {
        for (int i = 0; i < number_threads; i++) {
            HANDLE tid[i];
            tid[i] = CreateThread(NULL, 0, thread_lock_based_benchmark_set, &args, 0, NULL);
        }
    }
    //wait for join
    for(int i = 0;i<number_threads;i++){
        WaitForSingleObject (tid[i], INFINITE);
    }
    //calculate avg times between all threads and print them
    for(int i=0;i<number_mod;i++){
        float sum_overall=0;
        float sum_latency=0;
        for(int j=0;j<number_threads;j++) {
            sum_overall += overall_thread_time[j][i];
            sum_latency += mod_thread_latency[j][i];
        }
        overall_time[i] = sum_overall /(float)number_threads;
        mod_latency[i] = sum_latency /(float)number_threads;
        printf("overall lock based: %f for %d modifications\n",overall_time[i],i+1);
        printf("latency lock based: %f for %d modifications\n",mod_latency[i],i+1);
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

//Benchmark that single Thread does by stack data structure
DWORD WINAPI thread_lock_free_benchmark_stack(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lf,1);
    //cast arguments into usable arrays
    float (*latency)[k+1][bench_args->number_mod] = bench_args->latency;
    float (*overall)[k+1][bench_args->number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<bench_args->number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i % 2 == 0) {
                //push every even value and measure time
                tic_lat = clock();
                stack_push(&top, j);
                toc_lat = clock();
            } else {
                //pop when uneven and measure time
                tic_lat = clock();
                stack_pop(&top);
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
    return 0;
}

//Benchmark that single Thread does by stack data structure
DWORD WINAPI thread_lock_based_benchmark_stack(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lb,1);
    //cast arguments into usable arrays
    float (*latency)[k+1][bench_args->number_mod] = bench_args->latency;
    float (*overall)[k+1][bench_args->number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<bench_args->number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i % 2 == 0) {
                //push every even value and meassure time with locks
                tic_lat = clock();
                EnterCriticalSection(&mutex);
                lb_push(&stack, j);
                LeaveCriticalSection (&mutex);
                toc_lat = clock();
            } else {
                //pop when uneven and meassure time with locks
                tic_lat = clock();
                EnterCriticalSection(&mutex);
                lb_pop(&stack);
                LeaveCriticalSection (&mutex);
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
    return 0;
}


//Benchmark that single Thread does by queue data structure
DWORD WINAPI thread_lock_free_benchmark_queue(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lf,1);
    //cast arguments into usable arrays
    float (*latency)[k+1][bench_args->number_mod] = bench_args->latency;
    float (*overall)[k+1][bench_args->number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<bench_args->number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i % 2 == 0) {
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
    return 0;
}

//Benchmark that single Thread does by queue data structure
DWORD WINAPI thread_lock_based_benchmark_queue(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lb,1);
    //cast arguments into usable arrays
    float (*latency)[k+1][bench_args->number_mod] = bench_args->latency;
    float (*overall)[k+1][bench_args->number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<bench_args->number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i % 2 == 0) {
                //push every even value and meassure time with locks
                tic_lat = clock();
                EnterCriticalSection(&mutex);
                //lb_enqueue(&lb_queue, j); CMake need to be connected
                LeaveCriticalSection (&mutex);
                toc_lat = clock();
            } else {
                //pop when uneven and meassure time with locks
                tic_lat = clock();
                EnterCriticalSection(&mutex);
                //lb_dequeue(&lb_queue); CMake need to be connected
                LeaveCriticalSection (&mutex);
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
    return 0;
}

//Benchmark that single Thread does by set data structure
DWORD WINAPI thread_lock_free_benchmark_set(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lf,1);
    //cast arguments into usable arrays
    float (*latency)[k+1][bench_args->number_mod] = bench_args->latency;
    float (*overall)[k+1][bench_args->number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<bench_args->number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i % 2 == 0) {
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
    return 0;
}

//Benchmark that single Thread does by set data structure
DWORD WINAPI thread_lock_based_benchmark_set(void *args){
    //cast void pointer to argument pointer
    bench_args_t* bench_args = (bench_args_t*) args;
    //atomically reads thread counter value so only one thread accesses one array dimension
    int k = atomic_fetch_add((_Atomic int*)&count_lb,1);
    //cast arguments into usable arrays
    float (*latency)[k+1][bench_args->number_mod] = bench_args->latency;
    float (*overall)[k+1][bench_args->number_mod] = bench_args->overall;
    //start overall timer
    clock_t tic_overall = clock();
    for(int i=0;i<bench_args->number_mod;i++){
        float avg=0;
        float single_lat[i+1];
        for(int j=0;j<=i;j++) {
            clock_t tic_lat, toc_lat;
            if (i % 2 == 0) {
                //push every even value and meassure time with locks
                tic_lat = clock();
                EnterCriticalSection(&mutex);
                //insert(j); CMake need to be connected
                LeaveCriticalSection (&mutex);
                toc_lat = clock();
            } else {
                //pop when uneven and meassure time with locks
                tic_lat = clock();
                EnterCriticalSection(&mutex);
                //delete(j); CMake need to be connected
                LeaveCriticalSection (&mutex);
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
    return 0;
}





//old other Variants of Benchmark
void pushBenchmark(int threadCount, long* averagePush, long* totalTime) {
    LARGE_INTEGER start_time, end_time, pushstart_time, pushend_time;
    HANDLE tid[threadCount * 2];

    QueryPerformanceCounter(&start_time);
    QueryPerformanceCounter(&pushstart_time); // start clock for push timer
    for (int i = 0; i < threadCount; i++) // create push threads
        tid[i] = CreateThread(NULL, 0, push, &push, 0, NULL);
    for (int i = threadCount; i < threadCount * 2; i++) // create pop threads
        tid[i] = CreateThread(NULL, 0, pop, &pop, 0, NULL);
    WaitForMultipleObjects(threadCount, tid, TRUE, INFINITE); // join push threads
    QueryPerformanceCounter(&pushend_time);
    for (int i = threadCount; i < threadCount * 2; i++)
        WaitForSingleObject(tid[i], INFINITE); // join pop threads
    QueryPerformanceCounter(&end_time);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    *totalTime = ((end_time.QuadPart - start_time.QuadPart) * 1000000) / frequency.QuadPart;
    *averagePush = (((pushend_time.QuadPart - pushstart_time.QuadPart) * 1000000) / threadCount) / frequency.QuadPart;
}

void popBenchmark(int threadCount, long* averagePop, long* totalTime) {
    LARGE_INTEGER start_time, end_time, popstart_time, popend_time;
    HANDLE tid[threadCount * 2];

    QueryPerformanceCounter(&start_time);
    for (int i = 0; i < threadCount; i++) // create push threads
        tid[i] = CreateThread(NULL, 0, push, &push, 0, NULL);
    QueryPerformanceCounter(&popstart_time); // start timer for pop

    for (int i = threadCount; i < threadCount * 2; i++) // create pop threads
        tid[i] = CreateThread(NULL, 0, pop, &pop, 0, NULL);
    WaitForMultipleObjects(threadCount * 2, tid, TRUE, INFINITE); // join pop threads
    QueryPerformanceCounter(&popend_time); // stop timer for pop
    for (int i = 0; i < threadCount; i++) // join push threads
        WaitForSingleObject(tid[i], INFINITE);
    QueryPerformanceCounter(&end_time);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    *totalTime += ((end_time.QuadPart - start_time.QuadPart) * 1000000) / frequency.QuadPart;
    *averagePop = (((popend_time.QuadPart - popstart_time.QuadPart) * 1000000) / threadCount) / frequency.QuadPart;
}


//Functions for testing purposes
void * test1(){
    for(int i=0;i<100000000;i++){
        stack_push(&top,i);
    }
    return 0;
}

void * test2(){
    for(int i=0;i<50000000;i++){
        stack_pop(&top);
    }
    return 0;
}

void * test3(){
    for(int i=0;i<80000000;i++){
        stack_push(&top,i);
        if(i%2==0){
            stack_pop(&top);
        }

    }
    return 0;
}

void * test4(){
    for(int i=0;i<120000000;i++){
        stack_push(&top,i);
        if(i%5==0){
            stack_pop(&top);
        }
    }
    return 0;
}

//push and pop variations that the old Benchmark uses
DWORD WINAPI push(void *input)
{
    for(int i=0; i<100000; i++)
    {
        stack_push(&top, i);
        //printf("push %d\n",i);
    }
    ExitThread(0);
}

DWORD WINAPI pop(void *input)
{
    for(int i=0; i<100000;)
    {
        int result;
        result = stack_pop(&top);
        if(result == -1)
            //printf("the stack is empty\n");
            break;
        else
        {
            i++;
        }

    }
    ExitThread(0);
}




