cmake_minimum_required(VERSION 2.8.4)
project(AI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_directories(
/usr/lib/mysql/plugin/adt_null.so
/usr/lib/mysql/plugin/auth_test_plugin.so
/usr/lib/mysql/plugin/mypluglib.so
/usr/lib/mysql/plugin/qa_auth_server.so
/usr/lib/mysql/plugin/auth.so
/usr/lib/mysql/plugin/ha_example.so
/usr/lib/mysql/plugin/qa_auth_client.so
/usr/lib/mysql/plugin/semisync_master.so
/usr/lib/mysql/plugin/auth_socket.so
/usr/lib/mysql/plugin/libdaemon_example.so
/usr/lib/mysql/plugin/qa_auth_interface.so
/usr/lib/mysql/plugin/semisync_slave.so
)

set(SOURCE_FILES main.cpp)
add_executable(AI ${SOURCE_FILES})
