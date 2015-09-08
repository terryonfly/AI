cmake_minimum_required(VERSION 2.8.4)
project(AI)

MESSAGE(STSTUS "###################################")

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

MESSAGE(STSTUS "###################################")

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
