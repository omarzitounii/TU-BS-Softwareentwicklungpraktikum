//
// Created by Keanu Wirz on 22.05.23.
//

#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "lf_stack.h"
#include "lb_stack.h"

#include "lb_queue.h"
#include "M_S_Queue.h"

#include "lb_set.h"
#include "harrislist.h"

//#include "FreeStructure.h"



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

typedef struct benchmark_args_with_freelist_stack{
    bench_args_t* args;
    stackFreeStack* freeStack;
}bench_args_with_freelist_stack_t;

typedef struct benchmark_args_with_freelist_msqueue{
    bench_args_t* args;
    msqueueFreeStack* freeStack;
}bench_args_with_freelist_msqueue_t;

typedef struct benchmark_args_with_freelist_harris{
    bench_args_t* args;
    harrisFreeStack* freeStack;
}bench_args_with_freelist_harris_t;


//benchmark functions
int lock_free_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion);
int lock_based_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion);
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
/* Die muss nach dem gewählten datastruktur ausgewählt
void *test1(FreeStack *freeStack);
void *test2(FreeStack *freeStack);
void *test3(FreeStack *freeStack);
void *test4(FreeStack *freeStack);
 */

/*
 * benchmark Tests
 */
//lock free
void test_non_positive_thread_lock_free_benchmark();
void test_null_insertion_deletion_thread_lock_free_benchmark();
void test_negative_insertions_thread_lock_free_benchmark();
void test_negative_deletions_thread_lock_free_benchmark();

void test_nonzero_insertions_zero_deletions_thread_lock_free_benchmark();
void test_zero_insertions_nonzero_deletions_thread_lock_free_benchmark();
void test_nonzero_insertions_nonzero_deletions_thread_lock_free_benchmark();

//lock based
void test_non_positive_thread_lock_based_benchmark();
void test_null_insertion_deletion_thread_lock_based_benchmark();
void test_negative_insertions_thread_lock_based_benchmark();
void test_negative_deletions_thread_lock_based_benchmark();

void test_nonzero_insertions_zero_deletions_thread_lock_based_benchmark();
void test_zero_insertions_nonzero_deletions_thread_lock_based_benchmark();
void test_nonzero_insertions_nonzero_deletions_thread_lock_based_benchmark();

#endif //LOCKFREIESTACK_BENCHMARK_H
