//
// Created by Mohamed Amine, Omar & Keanu on 12.06.23.
//

#include <stdio.h>
//#include <pthread.h>
#include <Windows.h>
#include <time.h>

#include "lf_stack.h"
#include "lb_stack.h"

//CMake need to be connected: #include "M_S_Queue.h"
#include "lb_queue.h"
#include "M_S_Queue.h"

#include "lb_set.h"
#include "harrislist.h"

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
void lock_free_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion);
void lock_based_benchmark(enum data_structure structure, int number_threads, int number_insertion, int number_deletion);
DWORD WINAPI thread_lock_free_benchmark_stack(void *args);
DWORD WINAPI thread_lock_based_benchmark_stack(void *args);
DWORD WINAPI thread_lock_free_benchmark_queue(void *args);
DWORD WINAPI thread_lock_based_benchmark_queue(void *args);
DWORD WINAPI thread_lock_free_benchmark_set(void *args);
DWORD WINAPI thread_lock_based_benchmark_set(void *args);

//old alternative Benchmark functions
void pushBenchmark(int threadCount, long* averagePush, long* totalTime);
void popBenchmark(int threadCount, long* averagePop, long* totalTime);
DWORD WINAPI push(void *input);
DWORD WINAPI pop(void *input);

//functions used for testing
/*
void *test1(LfStack_FreeStack *lfStack_freeStack);
void *test2(LfStack_FreeStack *lfStack_freeStack);
void *test3(LfStack_FreeStack *lfStack_freeStack);
void *test4(LfStack_FreeStack *lfStack_freeStack);
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
