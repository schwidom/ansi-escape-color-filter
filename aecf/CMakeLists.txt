project(aecf)
cmake_minimum_required(VERSION 2.8)

add_compile_options( -Wall -std=c++14)
add_compile_options( -ggdb -O0)

aux_source_directory(src SRC_LIST)
aux_source_directory(src_tests SRC_TESTS_LIST)
aux_source_directory(src_tools SRC_TOOLS_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${SRC_TOOLS_LIST})
add_executable(tests ${SRC_TESTS_LIST} ${SRC_TOOLS_LIST})
# add_executable(tests ${SRC_TOOLS_LIST})

