/*#include <pybind11/pybind11.h>
#include <iostream>

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{}; // 启动Python解释器

    try {
        // 导入Python模块
        py::module_ your_script = py::module_::import("your_script");

        // 获取Python函数
        py::object add_func = your_script.attr("add");

        // 调用Python函数并传递参数
        py::object result = add_func(5, 3);

        // 获取返回值
        int value = result.cast<int>();
        std::cout << "Python function returned: " << value << std::endl;
    } catch (py::error_already_set& e) {
        std::cerr << "Python error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}*/

/*#include <Python.h>
#include <iostream>

int main() {
    // 初始化Python解释器
    Py_Initialize();
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter." << std::endl;
        return -1;
    }

    try {
        // 导入Python模块
        PyObject* pModule = PyImport_ImportModule("oo");
        if (!pModule) {
            throw std::runtime_error("Failed to import Python module.");
        }

        // 获取Python函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "add");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            throw std::runtime_error("Failed to get Python function.");
        }

        // 准备参数
        PyObject* pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyLong_FromLong(5));  // 参数1
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(3));  // 参数2

        // 调用Python函数
        PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
        if (!pResult) {
            throw std::runtime_error("Failed to call Python function.");
        }

        // 获取返回值
        long result = PyLong_AsLong(pResult);
        std::cout << "Python function returned: " << result << std::endl;

        // 清理
        Py_DECREF(pModule);
        Py_DECREF(pFunc);
        Py_DECREF(pArgs);
        Py_DECREF(pResult);
    } catch (const std::exception& e) {
        PyErr_Print();  // 打印Python错误信息
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 关闭Python解释器
    Py_Finalize();
    return 0;
}*/


/*#include <Python.h>
#include <iostream>
#include <cstdlib>  // 用于获取当前工作目录

int main() {
    // 初始化Python解释器
    Py_Initialize();
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter." << std::endl;
        return -1;
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
        Py_DECREF(pSys);
        Py_DECREF(pSysPath);
        Py_DECREF(pCwd);

        // 导入Python模块
        PyObject* pModule = PyImport_ImportModule("audio");
        if (!pModule) {
            throw std::runtime_error("Failed to import Python module.");
        }

        // 获取Python函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "add");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            throw std::runtime_error("Failed to get Python function.");
        }

        // 准备参数
        PyObject* pArgs = PyTuple_New(2);
        PyTuple_SetItem(pArgs, 0, PyLong_FromLong(5));  // 参数1
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(3));  // 参数2

        // 调用Python函数
        PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
        PyObject* pResult = PyObject_CallObject();
        if (!pResult) {
            throw std::runtime_error("Failed to call Python function.");
        }

        // 获取返回值
        long result = PyLong_AsLong(pResult);
        std::cout << "Python function returned: " << result << std::endl;

        // 清理
        Py_DECREF(pModule);
        Py_DECREF(pFunc);
        Py_DECREF(pArgs);
        Py_DECREF(pResult);
    } catch (const std::exception& e) {
        PyErr_Print();  // 打印Python错误信息
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 关闭Python解释器
    Py_Finalize();
    return 0;
}*/


/*#include <Python.h>
#include <iostream>
#include <cstdlib>  // 用于获取当前工作目录

int main() {
    // 初始化Python解释器
    Py_Initialize();
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter." << std::endl;
        return -1;
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
        Py_DECREF(pSys);
        Py_DECREF(pSysPath);
        Py_DECREF(pCwd);

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
        PyObject* pAudioFilePath = PyUnicode_FromString("/home/hcc/share/上海RMR-Major BGM 暂停_爱给网_aigei_com.mp3");
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
            PyObject* pItem = PyTuple_GetItem(pResultSplit, i);
            std::cout << PyUnicode_AsUTF8(pItem) << std::endl;
            PyList_Append(pAudioPaths, pItem);  // 将路径添加到列表
        }

        Py_DECREF(pFuncSplit);
        Py_DECREF(pArgsSplit);
        Py_DECREF(pResultSplit);

        // 调用 inference_with_dashscope 函数
        PyObject* pFuncInference = PyObject_GetAttrString(pModule, "inference_with_dashscope");
        if (!pFuncInference || !PyCallable_Check(pFuncInference)) {
            throw std::runtime_error("Failed to get Python function 'inference_with_dashscope'.");
        }

        PyObject* pPrompt = PyUnicode_FromString("请问这种音乐属于什么标签？它主要传达了什么情绪？能否推荐一些类似风格中文和英文音乐？");
        std::cout<<"===================\n";
        std::cout << PyUnicode_AsUTF8(pPrompt) << std::endl;
        std::cout<<"===================\n";
        //PyObject* path = PyUnicode_FromString("/home/hcc/share/上海RMR-Major BGM 暂停_爱给网_aigei_com.mp3");
        PyObject* pArgsInference = PyTuple_New(2);
        PyTuple_SetItem(pArgsInference, 0, pAudioPaths);
        //PyTuple_SetItem(pArgsInference, 0, path);
        PyTuple_SetItem(pArgsInference, 1, pPrompt);

        // 打印音频路径列表
        std::cout << "Audio paths passed to inference_with_dashscope:" << std::endl;
        for (Py_ssize_t i = 0; i < PyList_Size(pAudioPaths); ++i) {
            PyObject* pItem = PyList_GetItem(pAudioPaths, i);
            std::cout << PyUnicode_AsUTF8(pItem) << std::endl;
        }
        std::cout<<"----------------------------------------\n";

        PyObject* pResultInference = PyObject_CallObject(pFuncInference, pArgsInference);
        if (!pResultInference) {
            throw std::runtime_error("Failed to call Python function 'inference_with_dashscope'.");
        }

        Py_DECREF(pFuncInference);
        Py_DECREF(pArgsInference);
        Py_DECREF(pResultInference);
        Py_DECREF(pAudioPaths);
        Py_DECREF(pPrompt);

        Py_DECREF(pModule);
    } catch (const std::exception& e) {
        PyErr_Print();  // 打印Python错误信息
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 关闭Python解释器
    Py_Finalize();
    return 0;
}*/

#include <Python.h>
#include <iostream>
#include <cstdlib>  // 用于获取当前工作目录

int main() {
    // 初始化Python解释器
    Py_Initialize();
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter." << std::endl;
        return -1;
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
        Py_DECREF(pSys);
        Py_DECREF(pSysPath);
        Py_DECREF(pCwd);

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
        PyObject* pAudioFilePath = PyUnicode_FromString("/home/hcc/share/M800001em6GB0LiCvz.mp3");
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
            PyObject* pItem = PyTuple_GetItem(pResultSplit, i);
            std::cout << PyUnicode_AsUTF8(pItem) << std::endl;
            PyList_Append(pAudioPaths, pItem);  // 将路径添加到列表
        }

        Py_DECREF(pFuncSplit);
        Py_DECREF(pArgsSplit);
        //Py_DECREF(pResultSplit);

        // 调用 inference_with_dashscope 函数
        PyObject* pFuncInference = PyObject_GetAttrString(pModule, "inference_with_dashscope");
        if (!pFuncInference || !PyCallable_Check(pFuncInference)) {
            throw std::runtime_error("Failed to get Python function 'inference_with_dashscope'.");
        }

        PyObject* pPrompt = PyUnicode_FromString("将这首音乐按照音乐特点打上五个标签,只回复这个标签字段就行了");

        PyObject* pArgsInference = PyTuple_New(2);
        PyTuple_SetItem(pArgsInference, 0, pResultSplit);  // 第一个参数：音频路径列表
        PyTuple_SetItem(pArgsInference, 1, pPrompt);     // 第二个参数：提示字符串
        
        PyObject* pResultInference = PyObject_CallObject(pFuncInference, pArgsInference);
        if (!pResultInference) {
            throw std::runtime_error("Failed to call Python function 'inference_with_dashscope'.");
        }
        std::cout<<"----------------------\n";
        if (PyUnicode_Check(pResultInference)) {
            // 将返回值转换为 UTF-8 字符串
            const char* resultStr = PyUnicode_AsUTF8(pResultInference);
            std::cout << "Inference result: " << resultStr << std::endl;
        } else {
            std::cerr << "Unexpected return type from 'inference_with_dashscope'. Expected a string." << std::endl;
        }
        std::cout<<"----------------------\n";

        Py_DECREF(pFuncInference);
        Py_DECREF(pArgsInference);
        Py_DECREF(pResultInference);
        Py_DECREF(pAudioPaths);
        Py_DECREF(pPrompt);

        Py_DECREF(pModule);
    } catch (const std::exception& e) {
        PyErr_Print();  // 打印Python错误信息
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 关闭Python解释器
    Py_Finalize();
    return 0;
}