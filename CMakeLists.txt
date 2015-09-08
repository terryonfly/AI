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
    adt_null.so
    auth_test_plugin.so
    mypluglib.so
    qa_auth_server.so
    auth.so
    ha_example.so
    qa_auth_client.so
    semisync_master.so
    auth_socket.so
    libdaemon_example.so
    qa_auth_interface.so
    semisync_slave.so
)

set(SOURCE_FILES main.cpp)
add_executable(AI ${SOURCE_FILES})
