
#Created by Mohamed Amine & Omar on 12.06.23.

add_library(Queue main.c M_S_Queue.c ../header/M_S_Queue.h lb_queue.c ../header/lb_queue.h)


target_include_directories(Windows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})