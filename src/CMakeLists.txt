cmake_minimum_required(VERSION "3.18")
project("cgcs_vector")

set(CXX_STD "11") # C11
set(CFLAGS "-Wall -Werror -pedantic-errors")

set(CMAKE_CXX_STANDARD "${CXX_STD}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS}")

add_library("cgcs_vector" "cgcs_vector.h" "cgcs_vector.c")
target_include_directories("cgcs_vector" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
