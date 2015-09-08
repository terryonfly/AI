cmake_minimum_required(VERSION 2.8.4)
project(AI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_directories(
    /usr/lib/mysql/plugin
)

include_directories(
    /usr/include/mysql
)

link_libraries(
    libadt_null
    libauth_test_plugin
    libmypluglib
    libqa_auth_server
    libauth
    libha_example
    libqa_auth_client
    libsemisync_master
    libauth_socket
    liblibdaemon_example
    libqa_auth_interface
    libsemisync_slave
)

set(SOURCE_FILES main.cpp)
add_executable(AI ${SOURCE_FILES})
