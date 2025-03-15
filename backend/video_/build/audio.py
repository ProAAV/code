'''import dashscope
import time
import json
from pydub import AudioSegment
import os


def split_audio(audio_file_path, segment_length_ms=120000):
    
    audio = AudioSegment.from_file(audio_file_path)
    segment_paths = []

    for start in range(0, len(audio), segment_length_ms):
        end = start + segment_length_ms
        segment = audio[start:end]
        segment_file_name = f"segment_{start // 1000}.mp3"  # 生成分段文件名
        segment_file_path = os.path.abspath(segment_file_name)  # 获取绝对路径
        segment.export(segment_file_path, format="mp3")  # 导出分段文件

        segment_paths.append(segment_file_path)

    return segment_paths[:1]


def slow_print(text, delay=0.1):
    
    for char in text:
        print(char, end='', flush=True)
        time.sleep(delay)


def inference_with_dashscope(audio_paths, prompt):
    print("hyhyhyhyhy---------===========\n")
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
        model='qwen-omni-turbo',
        messages=messages,
        stream=True,
        incremental_output=True
    )
    complete_response = ""
    for response in responses:
        try:
            # 获取当前片段
            chunk = response["output"]["choices"][0]["message"]["content"][0]["text"]
            if chunk:
                complete_response+=chunk
                slow_print(chunk, delay=0.05)  # 逐字输出当前内容
        except Exception as e:
            pass
    return complete_response
if __name__ == '__main__':
    # 请用您的本地音频的绝对路径替换
    audio_file_path = "/root/Michita - Cry For Me (feat. Ami).mp3"
    prompt = """
    请根据以下固定的音乐分类标签，对所提供的音乐进行分类，并描述其主要传达的情绪。请仅使用下面列出的标签，不要添加或创建其他类别。分类标签：
    [古典音乐, 流行音乐（Pop）, 摇滚乐（Rock）, 嘻哈（Hip-Hop/Rap）, 电子音乐（Electronic）, 爵士乐（Jazz）, 蓝调（Blues）,
    民谣（Folk）, 乡村音乐（Country）, 拉丁音乐（Latin）, R&B（节奏与蓝调）, 重金属（Heavy Metal）, 朋克（Punk）, 世界音乐（World Music）]
    此外，请明确描述这首音乐所传达的情绪，如快乐、悲伤、激动、宁静等。"""
    audio_paths = split_audio(audio_file_path)
    inference_with_dashscope(audio_paths, prompt)
    '''

import os
import time
import base64
from pydub import AudioSegment
from openai import OpenAI

# Initialize OpenAI client
client = OpenAI(
    api_key="sk-ec776f72f2594f9184141aa91bd7b78e",
    base_url="https://dashscope.aliyuncs.com/compatible-mode/v1"
)


def encode_audio(audio_path):
    """Encode the audio file to base64 format."""
    with open(audio_path, "rb") as audio_file:
        return base64.b64encode(audio_file.read()).decode("utf-8")


'''def split_audio(audio_file_path, segment_length_ms=120000):
    """Split the audio file into smaller segments and return their paths."""
    audio = AudioSegment.from_file(audio_file_path)

    for start in range(0, len(audio), segment_length_ms):
        end = start + segment_length_ms
        segment = audio[start:end]
        segment_file_name = f"segment_{start // 1000}.mp3"
        segment_file_path = os.path.abspath(segment_file_name)
        segment.export(segment_file_path, format="mp3")

        return segment_file_path

    return None
'''
def split_audio(audio_file_path, segment_length_ms=120000):
    # 确保输出目录存在
    output_dir="/tmp/py/segs"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 加载音频文件
    audio = AudioSegment.from_file(audio_file_path)

    # 切割音频
    for start in range(0, len(audio), segment_length_ms):
        end = start + segment_length_ms
        segment = audio[start:end]

        # 生成保存的文件名和路径
        segment_file_name = f"segment_{start // 1000}.mp3"
        segment_file_path = os.path.join(output_dir, segment_file_name)

        # 导出音频片段
        segment.export(segment_file_path, format="mp3")

        # 返回当前片段的路径（如果需要）
        return segment_file_path

    return None  # 返回输出目录路径


def slow_print(text, delay=0.1):
    """Print text character by character with a delay."""
    for char in text:
        print(char, end='', flush=True)
        time.sleep(delay)


def inference_with_openai(base64_audio, prompt):
    """Send audio data to OpenAI API for inference."""
    # Prepare the messages for the OpenAI API
    messages = [
        {
            "role": "user",
            "content": [
                {
                    "type": "input_audio",
                    "input_audio": {
                        "data": f"data:;base64,{base64_audio}",
                        "format": "mp3",
                    },
                },
                {"type": "text", "text": prompt},
            ],
        },
    ]

    # Make the OpenAI API call
    responses = client.chat.completions.create(
        model="qwen-omni-turbo",
        messages=messages,
        modalities=["text"],
        # stream 必须设置为 True，否则会报错
        stream=True,
        stream_options={"include_usage": True},
    )
    result=""
    for response in responses:
        try:
            # 获取当前片段
            chunk = response.choices[0].delta.content
            result+=chunk
            slow_print(chunk)
        except Exception as e:
            pass
    return result

if __name__ == '__main__':
    # Replace with your local audio file path
    audio_file_path = "C:\\Users\\give4\\Desktop\\Qwen2.5-VL-main\\Michita - Cry For Me (feat. Ami).mp3"
    prompt = """
    请根据以下固定的音乐分类标签，对所提供的音乐进行分类，并描述其主要传达的情绪。请仅使用下面列出的标签，不要添加或创建其他类别。分类标签：
    [古典音乐, 流行音乐（Pop）, 摇滚乐（Rock）, 嘻哈（Hip-Hop/Rap）, 电子音乐（Electronic）, 爵士乐（Jazz）, 蓝调（Blues）,
    民谣（Folk）, 乡村音乐（Country）, 拉丁音乐（Latin）, R&B（节奏与蓝调）, 重金属（Heavy Metal）, 朋克（Punk）, 世界音乐（World Music）]
    此外，请明确描述这首音乐所传达的情绪，如快乐、悲伤、激动、宁静等。
    """

    # Split audio into smaller segments
    audio_path = split_audio(audio_file_path)

    # transform segments to base64_audio
    base64_audio = encode_audio(audio_path)

    # Run the inference with OpenAI
    inference_with_openai(base64_audio, prompt)
