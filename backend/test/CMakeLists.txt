cmake_minimum_required(VERSION 3.16)

project(ww)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cc
    
)
add_executable(eee ${SOURCES})
add_compile_options(-g)
include_directories(/usr/include/x86_64-linux-gnu/
                    /home/hcc/share/FastDfs/fastdfs/client
                    /home/hcc/share/FastDfs/fastdfs/tracker/
                    /home/hcc/share/FastDfs/fastdfs/common)

target_link_directories(eee PRIVATE /usr/lib/x86_64-linux-gnu)
target_link_libraries(eee PRIVATE avcodec avutil avformat swscale avdevice avfilter fdfsclient)

