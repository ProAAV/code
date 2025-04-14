#include"py_script.h"


#include <iostream>
#include <cstdlib>  // 用于获取当前工作目录

PyScript::PyScript(){
    
}
PyScript::~PyScript(){

}
int PyScript::pyscrpt_init(){
    // 初始化Python解释器
    if(!Py_IsInitialized()){
        Py_Initialize();
    }
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter." << std::endl;
        return -1;
    }
    return 0;
}
std::string PyScript::pyscrpt_audio_interface(const std::string& audio_file_path){
    // 初始化Python解释器

    //Py_Initialize();
    const char* resultStr=NULL;
    std::string resultStrCopy;
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter." << std::endl;
        return "";
    }
    try {
        // 获取当前工作目录
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == nullptr) {
            throw std::runtime_error("Failed to get current working directory.");
        }

        // 将当前工作目录添加到 sys.path
        PyObject* pSys = PyImport_ImportModule("sys");
        PyObject* pSysPath = PyObject_GetAttrString(pSys, "path");
        PyObject* pCwd = PyUnicode_FromString(cwd);
        PyList_Append(pSysPath, pCwd);
        /*Py_DECREF(pSys);
        Py_DECREF(pSysPath);
        Py_DECREF(pCwd);*/

        // 导入Python模块
        PyObject* pModule = PyImport_ImportModule("audio");
        if (!pModule) {
            throw std::runtime_error("Failed to import Python module.");
        }

        // 调用 split_audio 函数
        PyObject* pFuncSplit = PyObject_GetAttrString(pModule, "split_audio");
        if (!pFuncSplit || !PyCallable_Check(pFuncSplit)) {
            throw std::runtime_error("Failed to get Python function 'split_audio'.");
        }

        PyObject* pArgsSplit = PyTuple_New(2);
        PyObject* pAudioFilePath = PyUnicode_FromString(audio_file_path.c_str());
        PyObject* pSegmentLength = PyLong_FromLong(30000);

        PyTuple_SetItem(pArgsSplit, 0, pAudioFilePath);
        PyTuple_SetItem(pArgsSplit, 1, pSegmentLength);

        PyObject* pResultSplit = PyObject_CallObject(pFuncSplit, pArgsSplit);

        if (!pResultSplit) {
            throw std::runtime_error("Failed to call Python function 'split_audio'.");
        }
        std::cout << "Audio segments paths:" << std::endl;
        PyObject* pAudioPaths = PyList_New(0);  // 创建一个列表来存储路径
        for (Py_ssize_t i = 0; i < PyTuple_Size(pResultSplit); ++i) {
            std::cout<<"loop:"<<i<<'\n';
            PyObject* pItem = PyTuple_GetItem(pResultSplit, i);
            std::cout << PyUnicode_AsUTF8(pItem) << std::endl;
            PyList_Append(pAudioPaths, pItem);  // 将路径添加到列表
        }

        /*Py_DECREF(pFuncSplit);
        Py_DECREF(pArgsSplit);*/
        //Py_DECREF(pResultSplit);

        //调用encode_audio函数
        PyObject* pFuncEncode = PyObject_GetAttrString(pModule, "encode_audio");
        if (!pFuncEncode || !PyCallable_Check(pFuncEncode)) {
            throw std::runtime_error("Failed to get Python function 'encode_audio'.");
        }
        PyObject* pArgsEncode = PyTuple_New(1);
        PyTuple_SetItem(pArgsEncode, 0, pResultSplit);

        PyObject* pResultEncode = PyObject_CallObject(pFuncEncode, pArgsEncode);
        if (!pResultEncode) {
            throw std::runtime_error("Failed to call Python function 'encode_audio'.");
        }
        else{
            std::cout<<"pResultEncode exists\n";
        }

        if (PyUnicode_Check(pResultEncode)) {
            // 将返回值转换为 UTF-8 字符串
            resultStr = PyUnicode_AsUTF8(pResultEncode);
            //std::cout << "Inference result: " << resultStr << std::endl;
        } else {
            std::cerr << "Unexpected return type from 'encode_audio'. Expected a string." << std::endl;
        }
        // 调用 inference_with_dashscope 函数
        PyObject* pFuncInference = PyObject_GetAttrString(pModule, "inference_with_openai");
        if (!pFuncInference || !PyCallable_Check(pFuncInference)) {
            throw std::runtime_error("Failed to get Python function 'inference_with_dashscope'.");
        }

        PyObject* pPrompt = PyUnicode_FromString("请根据以下固定的音乐分类标签，对所提供的音乐进行分类，并描述其主要传达的情绪。请仅使用下面列出的标签，不要添加或创建其他类别。分类标签：\
        [古典音乐, 流行音乐（Pop）, 摇滚乐（Rock）, 嘻哈（Hip-Hop/Rap）, 电子音乐（Electronic）, 爵士乐（Jazz）, 蓝调（Blues）。\
        民谣（Folk）, 乡村音乐（Country）, 拉丁音乐（Latin）, R&B（节奏与蓝调）, 重金属（Heavy Metal）, 朋克（Punk）, 世界音乐（World Music）]\
        只需要回复五个标签字段不要回复其他任何词语或句子并且标签字段之间使用英文逗号分隔不许使用其他的符号，标签数一定得是五个不能多也不能少，严格按照我的要求来回复");
        //PyObject* pPrompt = PyUnicode_FromString("分析这个音频文件的特征，只要返回五个特征词给我，特征词之间使用逗号隔开");
        PyObject* pArgsInference = PyTuple_New(2);
        //PyTuple_SetItem(pArgsInference, 0, pResultSplit);  // 第一个参数：音频路径列表
        PyObject* pEncodeResult = PyUnicode_FromString(resultStr);
        PyTuple_SetItem(pArgsInference, 0, pEncodeResult);


        PyTuple_SetItem(pArgsInference, 1, pPrompt);     // 第二个参数：提示字符串
        
        PyObject* pResultInference = PyObject_CallObject(pFuncInference, pArgsInference);
        if (!pResultInference) {
            throw std::runtime_error("Failed to call Python function 'inference_with_dashscope'.");
        }
        else{
            std::cout<<"pResultInference exists\n";
        }
        std::cout<<"----------------------\n";
        
        if (PyUnicode_Check(pResultInference)) {
            // 将返回值转换为 UTF-8 字符串
            resultStr = PyUnicode_AsUTF8(pResultInference);
            //std::cout << "Inference result: " << resultStr << std::endl;
        } else {
            std::cerr << "Unexpected return type from 'inference_with_dashscope'. Expected a string." << std::endl;
        }

        resultStrCopy=resultStr;
        std::cout<<"----------------------\n";
        /*Py_DECREF(pResultSplit);
        Py_DECREF(pFuncInference);
        Py_DECREF(pArgsInference);
        Py_DECREF(pResultInference);
        Py_DECREF(pAudioPaths);
        Py_DECREF(pPrompt);

        Py_DECREF(pModule);*/

    } catch (const std::exception& e) {
        PyErr_Print();  // 打印Python错误信息
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 关闭Python解释器
    //Py_Finalize();
    //std::cout<<"res:"<<resultStrCopy<<"\n";
    return resultStrCopy;
}
std::string PyScript::pyscrpt_video_interface(const std::string& video_file_path){
        try {
            // 获取当前工作目录并添加到 sys.path
            //std::string prompt="结合文字、图图像等内容，认识别角色的情感状态和情感变化。并且按照左上角红色时间戳的HH:MM:SS组织时间,时间形式为**xx:xx:xx - xx:xx:xx**";
            std::string prompt="结合文字、图图像等内容，认识别角色的情感状态和情感变化。并且按照左上角红色时间戳的HH:MM:SS组织时间,按照'1. **00:00:00 - 00:00:43**\
            - 用户打开Corel VideoStudio X10软件，并选择了一个项目文件。\
            - 界面显示了多个素材窗口，用户在其中浏览并选择了几个视频和图片素材。\
            - 时间戳显示从00:00:00到00:00:43，期间用户主要是在浏览和选择素材。'这样的格式来返回结果";
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == nullptr) {
                throw std::runtime_error("Failed to get current working directory.");
            }
    
            PyObject* pSys = PyImport_ImportModule("sys");
            PyObject* pSysPath = PyObject_GetAttrString(pSys, "path");
            PyObject* pCwd = PyUnicode_FromString(cwd);
            PyList_Append(pSysPath, pCwd);
            /*Py_DECREF(pSys);
            Py_DECREF(pSysPath);
            Py_DECREF(pCwd);*/
    
            // 导入 Python 模块
            PyObject* pModule = PyImport_ImportModule("video");
            if (!pModule) {
                throw std::runtime_error("Failed to import Python module.");
            }
    
            // 获取 Python 函数
            PyObject* pFuncGetFrames = PyObject_GetAttrString(pModule, "get_video_frames");
            PyObject* pFuncInference = PyObject_GetAttrString(pModule, "inference_with_openai");
            if (!pFuncGetFrames || !PyCallable_Check(pFuncGetFrames)) {
                throw std::runtime_error("Failed to get Python function 'get_video_frames'.");
            }
            if (!pFuncInference || !PyCallable_Check(pFuncInference)) {
                throw std::runtime_error("Failed to get Python function 'inference_with_openai'.");
            }
    
            // 调用 get_video_frames 函数
            PyObject* pArgsGetFrames = PyTuple_New(2);
            PyObject* pVideoFilePath = PyUnicode_FromString(video_file_path.c_str());
            PyObject* pNumFrames = PyLong_FromLong(32); // 默认提取32帧
    
            PyTuple_SetItem(pArgsGetFrames, 0, pVideoFilePath);
            PyTuple_SetItem(pArgsGetFrames, 1, pNumFrames);
    
            PyObject* pResultGetFrames = PyObject_CallObject(pFuncGetFrames, pArgsGetFrames);
            if (!pResultGetFrames || !PyTuple_Check(pResultGetFrames)) {
                throw std::runtime_error("Failed to call Python function 'get_video_frames' or returned invalid data.");
            }
    
            // 提取 base64_frames 和 timestamps
            PyObject* pBase64Frames = PyTuple_GetItem(pResultGetFrames, 0);
            PyObject* pTimestamps = PyTuple_GetItem(pResultGetFrames, 1);
    
            // 调用 inference_with_openai 函数
            PyObject* pArgsInference = PyTuple_New(2);
            PyTuple_SetItem(pArgsInference, 0, pBase64Frames);
            PyTuple_SetItem(pArgsInference, 1, PyUnicode_FromString(prompt.c_str()));
    
            PyObject* pResultInference = PyObject_CallObject(pFuncInference, pArgsInference);
            if (!pResultInference) {
                throw std::runtime_error("Failed to call Python function 'inference_with_openai'.");
            }
    
            // 获取推理结果
            std::string resultStr;
            if (PyUnicode_Check(pResultInference)) {
                resultStr = PyUnicode_AsUTF8(pResultInference);
            } else {
                throw std::runtime_error("Unexpected return type from 'inference_with_openai'. Expected a string.");
            }
    
            // 清理资源
            /*Py_DECREF(pFuncGetFrames);
            Py_DECREF(pFuncInference);
            Py_DECREF(pModule);
            Py_DECREF(pArgsGetFrames);
            Py_DECREF(pResultGetFrames);
            Py_DECREF(pArgsInference);
            Py_DECREF(pResultInference);*/
            std::cout<<"resultStr:::::"<<resultStr<<'\n';
            return resultStr;
    
        } catch (const std::exception& e) {
            PyErr_Print();  // 打印 Python 错误信息
            std::cerr << "Error: " << e.what() << std::endl;
            return "";
        }
}
void pyscrpt_destory(){
    
}

