cmake_minimum_required(VERSION 3.13)
add_executable(server server.cpp XTcp.cpp)
add_executable(client client.cpp XTcp.cpp)
add_definitions(-std=c++11)
include_directories(.)

