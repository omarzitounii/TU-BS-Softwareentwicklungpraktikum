cmake_minimum_required(VERSION 3.25)
project(Mac_Version C)

set(CMAKE_C_STANDARD 11)

include_directories(Mac/src/Benchmark)
include_directories(Mac/src/Queue)
include_directories(Mac/src/Set)
include_directories(Mac/src/Stack)
include_directories(Mac/src/Free_Structure)

add_executable(Mac_Version
        Mac/src/Benchmark/benchmark.c
        Mac/src/Benchmark/benchmark.h
        Mac/src/Benchmark/main.c
        Mac/src/Queue/lb_queue.c
        Mac/src/Queue/lb_queue.h
        Mac/src/Queue/M_S_Queue.c
        Mac/src/Queue/M_S_Queue.h
        Mac/src/Queue/main.c
        Mac/src/Set/harrislist.c
        Mac/src/Set/harrislist.h
        Mac/src/Set/lb_set.c
        Mac/src/Set/lb_set.h
        Mac/src/Set/main.c
        Mac/src/Stack/lb_stack.c
        Mac/src/Stack/lb_stack.h
        Mac/src/Stack/lf_stack.c
        Mac/src/Stack/lf_stack.h
        Mac/src/Free_Structure/FreeStructure.c
        Mac/src/Free_Structure/FreeStructure.h)
