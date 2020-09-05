##  
##  \file       CMakeLists.txt
##  \brief      Set of instructions describing this project's sources/targets
##
##  \author     Gemuele Aludino
##  \date       05 Sep 2020
##

###########################################################################
##                                                                       ##
##  How to define a variable:                                            ##
##                                                                       ##
##  For a variable VAR_NAME,                                             ##
##  that is to have the value 'desired_value':                           ##
##                                                                       ##
##      set(VAR_NAME desired_value)                                      ##
##                                                                       ##
##  To use VAR_NAME as part of an argument,                              ##
##  such as in another variable's name or value,                         ##
##  you may do this (for a variable OTHER_NAME):                         ##
##                                                                       ##
##      set(OTHER_NAME, ${VAR_NAME}/the_other_part)                      ##
##                                                                       ##
###########################################################################

###########################################################################
##  Variables                                                            ##
###########################################################################

## --------------------------------------------------------------------- ##
## CMake version number, C version number
## --------------------------------------------------------------------- ##

set(CMAKE_VERSION_NUMBER "3.17.3")
set(C_STD "11")

## --------------------------------------------------------------------- ##
## Title for this project
## --------------------------------------------------------------------- ##

set(PROJECT_TITLE "cgcs_vector_ptr")

## --------------------------------------------------------------------- ##
## Top-level directory (folder) names
## (be sure to prefix ${DIR_*} to subdirectories within)
## --------------------------------------------------------------------- ##

set(DIR_INCLUDE "include")
set(DIR_CLIENT "client")        
set(DIR_SRC "src")
set(DIR_TESTS "tests")

## --------------------------------------------------------------------- ##
## Header file names (files within DIR_INCLUDE)
## --------------------------------------------------------------------- ##

##
## To add a header file,
## start with:
##      set(FILENAME_HEADER_NAME "header_name.h")
##
## It would look like this:
##      set(ALL_HEADERS
##          "${DIR_INCLUDE}/${FILENAME_HEADER_NAME}"
##      )
##
## If there are subdirectories between DIR_INCLUDE and your header file,
## be sure to specify those.
##

set(FILENAME_CGCS_VECTOR_PTR_H "cgcs_vector_ptr.h")

## Use ${ALL_HEADERS} to reference all .h files in DIR_INCLUDE
set(ALL_HEADERS 
    "${DIR_INCLUDE}/${FILENAME_CGCS_VECTOR_PTR_H}"
)

## --------------------------------------------------------------------- ##
## Source file names (files within DIR_SRC)
## --------------------------------------------------------------------- ##

set(FILENAME_CGCS_VECTOR_PTR_C "cgcs_vector_ptr.c")

## All .c files in DIR_SRC
set(ALL_SOURCES 
    "${DIR_SRC}/${FILENAME_CGCS_VECTOR_PTR_C}"
)

## --------------------------------------------------------------------- ##
## Client source file names (files within DIR_CLIENT)
## --------------------------------------------------------------------- ##

set(FILENAME_CGCS_VECTOR_PTR_MAIN_C "cgcs_vector_ptr_main.c")

## Full relative path of FILENAME_CGCS_VECTOR_PTR_MAIN
set(CGCS_VECTOR_PTR_MAIN "${DIR_CLIENT}/${FILENAME_CGCS_VECTOR_PTR_MAIN_C}")

## --------------------------------------------------------------------- ##
## Application (binary) title
## --------------------------------------------------------------------- ##

set(BIN_TITLE "cgcs_vptr_main")

## --------------------------------------------------------------------- ##
## Installation folder names
## --------------------------------------------------------------------- ##

set(DIR_BIN_INSTALL "bin")
set(DIR_SRC_INSTALL "src")

## --------------------------------------------------------------------- ##
## Flags
## --------------------------------------------------------------------- ##

set(WALL "-Wall")
set(WERR "-Werror")

set(PEDANTIC "-pedantic-errors")

set(CFLAGS "${WALL} ${WERR} ${PEDANTIC}")

###########################################################################
##  Configuration                                                        ##
###########################################################################

## --------------------------------------------------------------------- ##
## CMake version
## --------------------------------------------------------------------- ##

cmake_minimum_required(VERSION "${CMAKE_VERSION_NUMBER}")

## --------------------------------------------------------------------- ##
## C standard (-std=c11)
## --------------------------------------------------------------------- ##

set(CMAKE_C_STANDARD "${C_STD}")

## --------------------------------------------------------------------- ##
## Compiler flags
## --------------------------------------------------------------------- ##

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS}")

## --------------------------------------------------------------------- ##
## Project name (access via PROJECT_NAME)
## --------------------------------------------------------------------- ##

project("${PROJECT_TITLE}")

## --------------------------------------------------------------------- ##
## Header file path (.h/.hpp/.tpp)
## --------------------------------------------------------------------- ##

include_directories("${DIR_INCLUDE}")

## --------------------------------------------------------------------- ##
## Executables
## --------------------------------------------------------------------- ##

add_executable("${BIN_TITLE}" "${CGCS_VECTOR_PTR_MAIN}" "${ALL_HEADERS}" "${ALL_SOURCES}")

## --------------------------------------------------------------------- ##
## Installation directories (relative to CMAKE_INSTALL_PREFIX)
## --------------------------------------------------------------------- ##

### Binary
install(TARGETS ${BIN_TITLE} DESTINATION "${DIR_BIN_INSTALL}")

### Sources
install(FILES "${ALL_HEADERS}" "${CGCS_VECTOR_PTR_MAIN}" "${ALL_SOURCES}" DESTINATION "${DIR_SRC_INSTALL}")
