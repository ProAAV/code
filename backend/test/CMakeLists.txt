cmake_minimum_required(VERSION 3.16)

project(ww)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cc
)

add_compile_options(-g)
include_directories(/usr/include/x86_64-linux-gnu/libavcodec
                    /usr/include/x86_64-linux-gnu/libavutil
                    /usr/include/x86_64-linux-gnu/libavformat
                    /usr/include/x86_64-linux-gnu/libswscale)
add_executable(qq ${SOURCES})
target_link_directories(qq PRIVATE /usr/lib/x86_64-linux-gnu)
target_link_libraries(qq PRIVATE avcodec avutil avformat swscale)

