cmake_minimum_required(VERSION 3.0)
project(lockfreieStack C)

set(CMAKE_C_COMPILER "/usr/bin/clang")

set(sanitizeAddress ON)

set(CMAKE_C_STANDARD 11)

add_executable(lockfreieStack main.c lf_stack.h lf_stack.c benchmark.c benchmark.h lb_stack.c lb_stack.h)
#target_link_libraries(lockfreieStack M_S_Queue)
target_compile_options(lockfreieStack
PRIVATE
        -mcx16)

if(sanitizeAddress)
    # -fsanitize=address enables the clang address sanatizer
    # see http://clang.llvm.org/docs/AddressSanitizer.html for more details
    target_compile_options(lockfreieStack
            PRIVATE
            -fsanitize=address)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()