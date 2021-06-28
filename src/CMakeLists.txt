cmake_minimum_required(VERSION "3.18")
project("cgcs_vector")

set(C_STANDARD "11") # C11
set(CFLAGS "-Wall -Werror -pedantic-errors")

set(CMAKE_C_STANDARD ${C_STANDARD})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${CFLAGS})

add_library("cgcs_vector" "cgcs_vector.h" "cgcs_vector.c")
target_compile_options("cgcs_vector" PUBLIC "-fblocks")
target_include_directories("cgcs_vector" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
