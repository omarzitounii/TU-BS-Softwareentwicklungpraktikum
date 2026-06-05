#Created by Mohamed Amine & Omar on 13.06.23.

add_library(Queue main.c M_S_Queue.c ../header/M_S_Queue.h lb_queue.c ../header/lb_queue.h)

target_include_directories(Mac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})