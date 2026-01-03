cmake_minimum_required(VERSION 3.7)
project(http_server)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp src/HttpServer.cpp src/HttpServer.h src/utils.cpp src/utils.h)
add_executable(http_server ${SOURCE_FILES})