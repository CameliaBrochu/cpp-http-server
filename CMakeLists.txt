cmake_minimum_required(VERSION 3.24)
project(cpp_http_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cpp_http_server
        server.cpp
        tcpServer.cpp
        tcpServer.h)

if(MINGW)
    target_link_libraries(cpp_http_server ws2_32)
endif()
