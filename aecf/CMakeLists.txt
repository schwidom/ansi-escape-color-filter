project(aecf)
cmake_minimum_required(VERSION 2.8)

add_compile_options( -Wall -std=c++14)
add_compile_options( -ggdb -O0)

aux_source_directory( src SRC_LIST)
aux_source_directory( src_aecf SRC_AECF_LIST)
aux_source_directory( src_generators/arguments SRC_GEN_ARG_LIST)
aux_source_directory( src_tests SRC_TESTS_LIST)
aux_source_directory( src_tools SRC_TOOLS_LIST)

include(CMakePrintHelpers)
cmake_print_variables( ${SRC_LIST})
cmake_print_variables( ${SRC_AECF_LIST})
cmake_print_variables( ${SRC_TESTS_LIST})
cmake_print_variables( ${SRC_TOOLS_LIST})
cmake_print_variables( ${SRC_GEN_ARG_LIST})
cmake_print_variables( ${CMAKE_CURRENT_BINARY_DIR})
cmake_print_variables( ${CMAKE_CURRENT_SOURCE_DIR})

# recommendas vim setting: "set tags=./tags;/"
add_custom_target(tags
 COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q  ${CMAKE_CURRENT_SOURCE_DIR}
 WORKING_DIRECTORY ..)

add_executable( generate_arguments.bin ${SRC_GEN_ARG_LIST} ${SRC_TOOLS_LIST} src/AecfArgumentMap.cpp)

set( SRC_GENERATED_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src_generated/AecfArgConfig.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src_generated/AecfArgConfig.fpp ${CMAKE_CURRENT_SOURCE_DIR}/src_generated/AecfArgConfig.cpp)

add_custom_command( OUTPUT ${SRC_GENERATED_LIST}
 DEPENDS generate_arguments.bin
 COMMAND mkdir -p aecf/src_generated
 COMMAND aecf-debug/generate_arguments.bin
 COMMAND astyle ${SRC_GENERATED_LIST}
 WORKING_DIRECTORY ..)

set_source_files_properties( ${SRC_GENERATED_LIST}
 PROPERTIES GENERATED TRUE
)

add_custom_target( generate_arguments_target
 DEPENDS ${SRC_GENERATED_LIST}
)

# aux_source_directory( src_generated SRC_GENERATED_LIST)
# file( GLOB SRC_GENERATED_LIST src_generated/*)

add_executable( ${PROJECT_NAME} ${SRC_AECF_LIST} ${SRC_LIST} ${SRC_TOOLS_LIST} ${SRC_GENERATED_LIST} )
add_executable( tests ${SRC_TESTS_LIST} ${SRC_LIST} ${SRC_TOOLS_LIST} ${SRC_GENERATED_LIST} )

add_dependencies( ${PROJECT_NAME} generate_arguments_target)
add_dependencies( tests generate_arguments_target)


