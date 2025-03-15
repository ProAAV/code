#pragma once
#include <Python.h>
#include<string>
class PyScript{
public:
    PyScript();
    ~PyScript();
    int pyscrpt_init();
    void pyscrpt_destory();
    std::string pyscrpt_audio_interface(const std::string& audio_file_path);
    std::string pyscrpt_video_interface(const std::string& video_file_path);
private:

};