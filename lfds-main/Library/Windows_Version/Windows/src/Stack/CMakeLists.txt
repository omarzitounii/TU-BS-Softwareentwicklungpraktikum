
#Created by Mohamed Amine & Omar on 12.06.23.

add_library(Stack lb_stack.c ../header/lb_stack.h lf_stack.c ../header/lf_stack.h)

target_include_directories(Windows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


