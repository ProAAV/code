



cmake_minimum_required(VERSION 3.16)

project(video)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cc
    base/net_event_loop.cc
    base/net_socket.cc 
    base/config_read.cc
    base/mongoose.c
    base/http_layer.cc
    api/api_upload.cc
    base/file_info.cc
    base/mysqlconn.cc
    api/api_files_list.cc
    api/api_login.cc
    api/api_register.cc
    api/api_update_user_history.cc
    api/api_user_info.cc
    api/api_search.cc
    api/api_search_log.cc
    api/api_get_search_log.cc
    base/py_script.cc
    api/api_list_loop.cc
    api/api_analysis_messages.cc
)
add_executable(video_server ${SOURCES})
add_compile_options(-g)
include_directories(/usr/include/x86_64-linux-gnu/
                    /home/hcc/share/FastDfs/fastdfs/client
                    /home/hcc/share/FastDfs/fastdfs/tracker/
                    /home/hcc/share/FastDfs/fastdfs/common
                    /usr/include/python3.8
                    )

target_link_directories(video_server PRIVATE /usr/lib/x86_64-linux-gnu /usr/lib/python3.8/config-3.8-x86_64-linux-gnu /usr/lib)
target_link_libraries(video_server PRIVATE avcodec avutil avformat swscale avdevice avfilter fdfsclient mysqlclient jsoncpp python3.8 crypt pthread dl util m)