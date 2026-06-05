cmake_minimum_required(VERSION 3.22)
#set(CMAKE_C_COMPILER "c:/Program Files/LLVM/bin/clang.exe")
project(M_S_Queue C)

set(sanitizeAddress ON)

set(CMAKE_C_STANDARD 11)

add_executable(M_S_Queue main.c M_S_Queue.h M_S_Queue.c lb_queue.c lb_queue.h)
#add_library(libatomic SHARED IMPORTED)
#target_link_libraries(M_S_Queue libatomic)

if(sanitizeAddress)
    # -fsanitize=address enables the clang address sanitizer
    # see http://clang.llvm.org/docs/AddressSanitizer.html for more details
    target_compile_options(M_S_Queue
            PRIVATE
            -fsanitize=address
            -mcx16)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address ")
endif()
