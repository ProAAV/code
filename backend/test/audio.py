'''import dashscope
import time
import json
from pydub import AudioSegment
import os


def split_audio(audio_file_path, segment_length_ms=30000):
    """将音频文件分割成多个片段并返回片段的绝对路径"""
    audio = AudioSegment.from_file(audio_file_path)
    segment_paths = []

    for start in range(0, len(audio), segment_length_ms):
        end = start + segment_length_ms
        segment = audio[start:end]
        segment_file_name = f"segment_{start // 1000}.mp3"  # 生成分段文件名
        segment_file_path = os.path.abspath(segment_file_name)  # 获取绝对路径
        segment.export(segment_file_path, format="mp3")  # 导出分段文件

        segment_paths.append(segment_file_path)

    return segment_paths[:3]


def slow_print(text, delay=0.1):
    """逐字打印文本"""
    for char in text:
        print(char, end='', flush=True)
        time.sleep(delay)


def inference_with_dashscope(audio_paths, prompt):
    # 构建消息结构
    messages = [
        {
            "role": "user",
            "content": [{"audio": path} for path in audio_paths] + [{"text": prompt}]
        }
    ]

    # 调用Dashscope API
    responses = dashscope.MultiModalConversation.call(
        api_key="sk-ec776f72f2594f9184141aa91bd7b78e",  # 从环境变量中获取API密钥,或者直接配置
        model='qwen-audio-turbo-latest',
        messages=messages,
        stream=True,
        incremental_output=True
    )

    for response in responses:
        try:
            # 获取当前片段
            chunk = response["output"]["choices"][0]["message"]["content"][0]["text"]
            if chunk:
                slow_print(chunk, delay=0.05)  # 逐字输出当前内容
        except Exception as e:
            pass

if __name__ == '__main__':
    # 请用您的本地音频的绝对路径替换
    audio_file_path = "/home/hcc/share/上海RMR-Major BGM 暂停_爱给网_aigei_com.mp3"
    prompt = "请问这种音乐属于什么标签？它主要传达了什么情绪？能否推荐一些类似风格中文和英文音乐？"
    audio_paths = split_audio(audio_file_path)
    inference_with_dashscope(audio_paths, prompt)
    '''


import dashscope
import time
import json
from pydub import AudioSegment
import os

def split_audio(audio_file_path, segment_length_ms=30000):

    print("audio_file_path:", audio_file_path)  # 打印接收到的音频路径
    print("segment_length_ms:", segment_length_ms) 
    """将音频文件分割成多个片段并返回片段的绝对路径"""
    audio = AudioSegment.from_file(audio_file_path)
    segment_paths = []

    for start in range(0, len(audio), segment_length_ms):
        end = start + segment_length_ms
        segment = audio[start:end]
        segment_file_name = f"segment_{start // 1000}.mp3"  # 生成分段文件名
        segment_file_path = os.path.abspath(segment_file_name)  # 获取绝对路径
        segment.export(segment_file_path, format="mp3")  # 导出分段文件

        segment_paths.append(segment_file_path)

    return segment_paths[:3]  # 返回前3个片段路径

def inference_with_dashscope(audio_paths, prompt):

    print("Received audio paths:", audio_paths)  # 打印接收到的音频路径
    print("Received prompt:", prompt) 
    # 构建消息结构
    messages = [
        {
            "role": "user",
            "content": [{"audio": path} for path in audio_paths] + [{"text": prompt}]
        }
    ]

    # 调用Dashscope API
    responses = dashscope.MultiModalConversation.call(
        api_key="sk-ec776f72f2594f9184141aa91bd7b78e",  # 替换为你的API密钥
        model='qwen-audio-turbo-latest',
        messages=messages,
        stream=True,
        incremental_output=True
    )
    result=""
    for response in responses:
        try:
            # 检查 response 是否为空
            if response is None or "output" not in response:
                print("No valid response received.")
                continue

            # 安全地访问嵌套字段
            output = response.get("output", {})
            choices = output.get("choices", [])
            if not choices:
                print("No choices in response.")
                continue

            message = choices[0].get("message", {})
            content = message.get("content", [])
            if not content:
                print("No content in message.")
                continue

            chunk = content[0].get("text", "")
            result=result+chunk
            if chunk:
                print(chunk, end='', flush=True)  # 逐字输出当前内容
            
        except Exception as e:
            print(f"Error in response: {e}")
    return result

if __name__ == '__main__':
    # 请用您的本地音频的绝对路径替换
    audio_file_path = "/home/hcc/share/上海RMR-Major BGM 暂停_爱给网_aigei_com.mp3"
    prompt = "请问这种音乐属于什么标签？它主要传达了什么情绪？能否推荐一些类似风格中文和英文音乐？"
    audio_paths = split_audio(audio_file_path)
    inference_with_dashscope(audio_paths, prompt)