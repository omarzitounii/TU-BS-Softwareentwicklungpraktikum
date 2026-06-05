//
// Created by Keanu Wirz on 22.05.23.
//

#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "lf_stack.h"
#include "lb_stack.h"

//CMake need to be connected: #include "M_S_Queue.h"
//#include "lb_queue.h"

//CMake need to be connected: #include "harrislist.h"
//#include "lb_set.h"


#ifndef LOCKFREIESTACK_BENCHMARK_H
#define LOCKFREIESTACK_BENCHMARK_H

//structure
enum data_structure {STACK=0, QUEUE=1, SET=2};

//struct used to store arguments for the Benchmark function
typedef struct benchmark_args{
    void* overall;
    void* latency;
    int i;
    _Atomic int number_insertion;
    _Atomic int number_deletion;
    enum data_structure structure;
}bench_args_t;

typedef struct benchmark_args_with_freelist{
    bench_args_t* args;
    FreeStack* freeStack;
}bench_args_with_freelist_t;

//benchmark functions
void lock_free_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion);
void lock_based_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion);
void *thread_lock_free_benchmark_stack(void *args);
void *thread_lock_based_benchmark_stack(void *args);
void *thread_lock_free_benchmark_queue(void *args);
void *thread_lock_based_benchmark_queue(void *args);
void *thread_lock_free_benchmark_set(void *args);
void *thread_lock_based_benchmark_set(void *args);


//old alternative Benchmark functions
void pushBenchmark(int threadCount, long* averagePush, long* totalTime);
void popBenchmark(int threadCount, long* averagePop, long* totalTime);
void *push(void *input);
void *pop(void *input);

//functions used for testing
void *test1(FreeStack *freeStack);
void *test2(FreeStack *freeStack);
void *test3(FreeStack *freeStack);
void *test4(FreeStack *freeStack);

#endif //LOCKFREIESTACK_BENCHMARK_H
