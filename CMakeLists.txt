cmake_minimum_required(VERSION 2.8.4)
project(AI)

########## for windows ##########
IF(WIN32)
	MESSAGE(STATUS "At Windows OS")

    include_directories(
        #
    )

    link_directories(
        #
    )

########## for mac ##########
ELSEIF(APPLE)
	MESSAGE(STATUS "At Mac OS")

    include_directories(
        /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/include
    )

    link_directories(
        /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib
    )

########## for linux ##########
ELSEIF(UNIX)
	MESSAGE(STATUS "At Linux OS")

    include_directories(
        /usr/include/mysql
    )

    link_directories(
        #
    )

ENDIF()

link_libraries(
    mysqlclient
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(AI ${SOURCE_FILES})
