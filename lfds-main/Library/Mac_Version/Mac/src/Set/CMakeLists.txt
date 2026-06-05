#Created by Mohamed Amine & Omar on 13.06.23.

add_library(Set main.c harrislist.c ../header/harrislist.h lb_set.c ../header/lb_set.h)

target_include_directories(Mac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})