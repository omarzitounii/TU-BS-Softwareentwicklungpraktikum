#Created by Mohamed Amine & Omar on 13.06.23.

add_executable(Windows main.c benchmark.c ../header/benchmark.h)

#target_compile_options(Windows
#        PRIVATE
#        -fsanitize=address)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")



target_link_libraries(Windows Stack)
target_link_libraries(Windows Queue)
target_link_libraries(Windows Set)

