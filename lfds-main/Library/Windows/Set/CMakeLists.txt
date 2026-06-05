cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "/usr/bin/clang")

project(harrislist C)

set(sanitizeAddress ON)

set(CMAKE_C_STANDARD 11)

SET(CMAKE_BUILD_TYPE Debug)

add_executable(harrislist main.c harrislist.c harrislist.h)

if(sanitizeAddress)
    # -fsanitize=address enables the clang address sanatizer
    # see http://clang.llvm.org/docs/AddressSanitizer.html for more details
    target_compile_options(harrislist
            PRIVATE
            -fsanitize=address)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address ")
endif()