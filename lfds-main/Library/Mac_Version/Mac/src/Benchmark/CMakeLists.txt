#Created by Mohamed Amine & Omar on 13.06.23.

add_executable(Mac main.c benchmark.c ../header/benchmark.h)

target_compile_options(Mac

PRIVATE -fsanitize=address)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

target_link_libraries(Mac Stack)
target_link_libraries(Mac Queue)
target_link_libraries(Mac Set)
target_link_libraries(Mac Free_Structure)
