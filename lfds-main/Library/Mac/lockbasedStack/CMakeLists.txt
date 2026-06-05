cmake_minimum_required(VERSION 3.0)
project(lockbasedStack C)

set(CMAKE_C_COMPILER "/usr/bin/clang")

#Only one sanitizer may be enabled at any time
set(sanitizeThread  OFF)
set(sanitizeAddress ON)

set(CMAKE_C_STANDARD 11)

add_executable(lockbasedStack main.c stack.h stack.c benchmark.c benchmark.h)

if(sanitizeThread)
    # -fsanitize=thread enables the clang thread sanitizer
    # see http://clang.llvm.org/docs/ThreadSanitizer.html for more details
    target_compile_options(lockbasedStack
            PRIVATE
            -fsanitize=thread
            -fPIE)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()
if(sanitizeAddress)
    # -fsanitize=address enables the clang address sanatizer
    # see http://clang.llvm.org/docs/AddressSanitizer.html for more details
    target_compile_options(lockbasedStack
            PRIVATE
            -fsanitize=address)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()
