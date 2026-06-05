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
    _Atomic int number_mod;
    enum data_structure structure;
}bench_args_t;

//benchmark functions
void lock_free_benchmark(enum data_structure structure, int number_threads, int number_mod);
void lock_based_benchmark(enum data_structure structure, int number_threads, int number_mod);
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
void *test1();
void *test2();
void *test3();
void *test4();

#endif //LOCKFREIESTACK_BENCHMARK_H
