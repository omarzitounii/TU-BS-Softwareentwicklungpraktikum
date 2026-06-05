
#Created by Mohamed Amine & Omar on 12.06.23.

cmake_minimum_required(VERSION 3.0)
project(Windows C)

set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/bin/clang-cl.exe")

#set(sanitizeAddress ON)

set(CMAKE_C_STANDARD 11)
set(sanitizeThread  OFF)
set(sanitizeAddress OFF)

set(CMAKE_C_STANDARD 11)
#set(BUILD_SHARED_LIBS OFF)
##set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")



#set(REPLACE "/MD" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
if(MSVC)
    add_compile_options(/W4 /EHs)
else()
    add_compile_options(-Wall -Wextra -pedantic -mcx16)
endif()

if(xxxMSVC)
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
endif()

add_subdirectory(src)
#target_link_libraries(lockfreieStack M_S_Queue)
target_compile_options(Windows
        PRIVATE
        -mcx16)

if(sanitizeAddress)
    # -fsanitize=address enables the clang address sanatizer
    # see http://clang.llvm.org/docs/AddressSanitizer.html for more details
    target_compile_options(Windows
            PRIVATE
            -fsanitize=address)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()