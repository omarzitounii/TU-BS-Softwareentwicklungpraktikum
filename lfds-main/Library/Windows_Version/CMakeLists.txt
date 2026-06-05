cmake_minimum_required(VERSION 3.25)
project(Windows_Version C)

set(CMAKE_C_STANDARD 11)

include_directories(Windows/src/Benchmark)
include_directories(Windows/src/Queue)
include_directories(Windows/src/Set)
include_directories(Windows/src/Stack)
include_directories(Windows/src/Free_Structure)

add_executable(Windows_Version
        Windows/src/Benchmark/benchmark.c
        Windows/src/header/benchmark.h
        Windows/src/Benchmark/main.c
        Windows/src/Queue/lb_queue.c
        Windows/src/header/lb_queue.h
        Windows/src/Queue/M_S_Queue.c
        Windows/src/header/M_S_Queue.h
        Windows/src/Queue/main.c
        Windows/src/Set/harrislist.c
        Windows/src/header/harrislist.h
        Windows/src/Set/lb_set.c
        Windows/src/header/lb_set.h
        Windows/src/Set/main.c
        Windows/src/Stack/lb_stack.c
        Windows/src/header/lb_stack.h
        Windows/src/Stack/lf_stack.c
        Windows/src/header/lf_stack.h)

