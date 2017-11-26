# das neuerzeugen der cmake zieldateien (bisher per bin/cmake.sh)
# ... wird auch durch das Makefile mit abgedeckt
cmake_minimum_required(VERSION 3.0)
# add_definitions( -std=c++14) # eigentlich nur fuer -D...
add_compile_options( -Wall -std=c++14)
add_compile_options( -ggdb -O0)
add_custom_command(OUTPUT tags 
 COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q src*
 DEPENDS src*/*)
add_executable(aecf tags src/main.cpp)
add_executable(tests tags src_tests/main.cpp)
