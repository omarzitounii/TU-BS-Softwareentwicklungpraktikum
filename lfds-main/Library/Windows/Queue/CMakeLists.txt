cmake_minimum_required(VERSION 3.22)
#set(CMAKE_C_COMPILER "c:/Program Files/LLVM/bin/clang.exe")
#set(CMAKE_C_FLAGS "/experimental:c11atomics")
project(M_S_Queue C)

set(sanitizeThread  OFF)
set(sanitizeAddress OFF)

set(CMAKE_C_STANDARD 11)
#set(BUILD_SHARED_LIBS OFF)
##set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


#set(REPLACE "/MD" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})

if(xxxMSVC)
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
endif()



add_executable(M_S_Queue main.c M_S_Queue.h M_S_Queue.c)
#add_library(libatomic SHARED IMPORTED)
#target_link_libraries(M_S_Queue libatomic)

if(sanitizeThread)
    # -fsanitize=thread enables the clang thread sanitizer
    # see http://clang.llvm.org/docs/ThreadSanitizer.html for more details
    target_compile_options(M_S_Queue
            PRIVATE
            -fsanitize=thread
            -fPIE)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()
if(sanitizeAddress)
    # -fsanitize=address enables the clang address sanitizer
    # see http://clang.llvm.org/docs/AddressSanitizer.html for more details
    target_compile_options(M_S_Queue
            PRIVATE
            -fsanitize=address)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /fsanitize=address ")
endif()


