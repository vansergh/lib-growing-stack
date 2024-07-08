cmake_minimum_required(VERSION 3.5)

#include cmake functions
include(cmake/functions.cmake)

# Include list of files to be built
set(SOURCES_INCLUDE_PATH *.cpp *.c *.cxx)
set(HEADERS_INCLUDE_PATH *.hpp *.h)

# Exclude list of files (regxp)
set(EXCLUDE_PATH "(/out|/opt|/res)")

# Converting current folder name to PROJECT_NAME
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
set (PROJECT_NAME ${CURRENT_FOLDER})

# Set C++ Standart
set(CMAKE_CXX_STANDARD 17)

# Set project name variable
project(${PROJECT_NAME})

# Checking if CMakeLists.txt exists in project folders and include it
#set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
#if(EXISTS ${SRC}/CMakeLists.txt)
#add_subdirectory(${SRC})
#endif()

#set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#if(EXISTS ${LIB}/CMakeLists.txt)
#add_subdirectory(${LIB})
#endif()

#set(OPT ${CMAKE_CURRENT_SOURCE_DIR}/opt)
#if(EXISTS ${OPT}/CMakeLists.txt)
#add_subdirectory(${OPT})
#endif()

#set(RES ${CMAKE_CURRENT_SOURCE_DIR}/res)
#if(EXISTS ${RES}/CMakeLists.txt)
#add_subdirectory(${RES})
#endif()

# Search of all sources and headers files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCES_INCLUDE_PATH})
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/${HEADERS_INCLUDE_PATH})

# Filter sources and headers to build without ignore list
FilterRegex(EXCLUDE ${EXCLUDE_PATH} SOURCES ${SOURCES})
FilterRegex(EXCLUDE ${EXCLUDE_PATH} HEADERS ${HEADERS})

# Print the list of files collected
string(REPLACE ";"  "\n" HEADERS_STR "${HEADERS}")
string(REPLACE ";"  "\n" SOURCES_STR "${SOURCES}")
message(STATUS  "Found headers:")
list(APPEND CMAKE_MESSAGE_INDENT "\t\t")
message(STATUS "${HEADERS_STR}")
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(STATUS  "Found sources:")
list(APPEND CMAKE_MESSAGE_INDENT "\t\t")
message(STATUS  "${SOURCES_STR}")

# Build executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})