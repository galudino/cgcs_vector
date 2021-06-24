cmake_minimum_required(VERSION "3.18")
project("cgcs_vector_demo")

set(C_STANDARD "11")
set(CFLAGS "-Wall -Werror -pedantic-errors")

set(CMAKE_C_STANDARD ${C_STANDARD})
set(CMAKE_C_FLAGS ${CFLAGS})

add_executable("cgcs_vector_demo" "cgcs_vector_demo.c")
target_link_libraries("cgcs_vector_demo" LINK_PUBLIC "cgcs_vector")
