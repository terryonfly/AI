cmake_minimum_required(VERSION 2.8.4)
project(AI)

IF (WIN32)
	MESSAGE(STATUS "Now is windows")
ELSEIF (APPLE)
	MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")
ENDIF ()

########## for linux ##########

#include_directories(
#    /usr/include/mysql
#)
#
#link_directories(
#
#)

########## for mac ##########

include_directories(
    /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/include
)

link_directories(
    /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib
)

########## common ##########

link_libraries(
    mysqlclient
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(AI ${SOURCE_FILES})
