cmake_minimum_required(VERSION 3.16)

project(video)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cc
    base/net_event_loop.cc
    base/net_socket.cc 
    base/config_read.cc
)
add_executable(video_server ${SOURCES})
add_compile_options(-g)

