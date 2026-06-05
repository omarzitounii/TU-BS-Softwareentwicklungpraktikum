//
// Created by Nazli on 23.05.23.
//
#include "stack.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#ifndef LOCKBASEDSTACK_BENCHMARK_H
#define LOCKBASEDSTACK_BENCHMARK_H
void* push_thread(void *arg);
void* pop_thread(void *arg);
void pushBenchmark(int threadCount, long* averagePush, long* totalTime);
void popBenchmark(int threadCount, long* averagePop, long* totalTime);
#endif //LOCKBASEDSTACK_BENCHMARK_H
