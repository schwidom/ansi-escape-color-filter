project(aecf)
cmake_minimum_required(VERSION 2.8)

add_compile_options( -Wall -std=c++14)
add_compile_options( -ggdb -O0)

aux_source_directory( src SRC_LIST)
aux_source_directory( src_aecf SRC_AECF_LIST)
aux_source_directory( src_tests SRC_TESTS_LIST)
aux_source_directory( src_tools SRC_TOOLS_LIST)

include(CMakePrintHelpers)
cmake_print_variables( ${SRC_LIST})
cmake_print_variables( ${SRC_AECF_LIST})
cmake_print_variables( ${SRC_TESTS_LIST})
cmake_print_variables( ${SRC_TOOLS_LIST})
cmake_print_variables( ${CMAKE_CURRENT_BINARY_DIR})
cmake_print_variables( ${CMAKE_CURRENT_SOURCE_DIR})

add_executable( ${PROJECT_NAME} ${SRC_AECF_LIST} ${SRC_LIST} ${SRC_TOOLS_LIST})
add_executable( tests ${SRC_TESTS_LIST} ${SRC_LIST} ${SRC_TOOLS_LIST})

