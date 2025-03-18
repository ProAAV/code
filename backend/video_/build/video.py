import os
import numpy as np
from PIL import Image, ImageDraw, ImageFont
import decord
import io
import base64
from decord import VideoReader, cpu
from openai import OpenAI
import time


# 获取视频帧的函数，并将每帧图像转为 Base64 编码
def get_video_frames(video_path, num_frames=128):
    # 读取本地视频
    vr = VideoReader(video_path, ctx=cpu(0))
    total_frames = len(vr)
    fps = vr.get_avg_fps()

    # 均匀选择需要提取的帧的索引
    indices = np.linspace(0, total_frames - 1, num=num_frames, dtype=int)
    frames = vr.get_batch(indices).asnumpy()

    # 计算时间戳
    timestamps = indices / fps  # 时间戳单位为秒

    # 在每帧上叠加时间戳，并转换为 Base64 编码
    base64_frames = []
    for i in range(num_frames):
        formatted_ts = format_timestamp(timestamps[i])
        pil_img = Image.fromarray(frames[i])
        frames[i] = np.array(
            overlay_timestamp(pil_img, formatted_ts, position=(10, 10), font_size=100, color=(255, 0, 0)))

        # 将每帧转换为 base64 编码
        buffered = io.BytesIO()
        pil_img.save(buffered, format="JPEG")  # 可以选择 PNG 格式
        img_base64 = base64.b64encode(buffered.getvalue()).decode('utf-8')
        base64_frames.append(img_base64)

    return base64_frames, timestamps


def overlay_timestamp(image, timestamp, position=(10, 10), font_path="/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", font_size=100,
                      color=(255, 0, 0)):
    """
    在图像上叠加时间戳文本。

    :param image: PIL.Image 对象
    :param timestamp: 时间戳文本，例如 "00:01:23"
    :param position: 文本位置，默认为左上角 (10, 10)
    :param font_path: 字体路径，如果为 None 则使用默认字体
    :param font_size: 字体大小
    :param color: 文本颜色，默认白色
    :return: 带有时间戳的 PIL.Image 对象
    """
    draw = ImageDraw.Draw(image)
    try:
        if font_path and os.path.exists(font_path):
            font = ImageFont.truetype(font_path, font_size)
        else:
            font = ImageFont.load_default()
    except Exception as e:
        print(f"加载字体时出错: {e}")
        font = ImageFont.load_default()

    # 添加文本的阴影以提高可读性
    shadow_color = (0, 0, 0)
    x, y = position
    # 绘制阴影
    draw.text((x + 1, y + 1), timestamp, font=font, fill=shadow_color)
    # 绘制文本
    draw.text(position, timestamp, font=font, fill=color)
    return image


def format_timestamp(seconds):
    """
    将秒数格式化为 "HH:MM:SS" 格式的字符串。

    :param seconds: 秒数
    :return: 格式化后的时间字符串
    """
    hrs = int(seconds // 3600)
    mins = int((seconds % 3600) // 60)
    secs = int(seconds % 60)
    return f"{hrs:02d}:{mins:02d}:{secs:02d}"


# 配置 OpenAI 客户端
client = OpenAI(
    api_key="sk-ec776f72f2594f9184141aa91bd7b78e",  # 使用环境变量中的 API Key
    base_url="https://dashscope.aliyuncs.com/compatible-mode/v1",
)


def slow_print(text, delay=0.1):
    """Print text character by character with a delay."""
    for char in text:
        print(char, end='', flush=True)
        time.sleep(delay)


def inference_with_openai(base64_frames, prompt):
    """Send audio data to OpenAI API for inference."""
    # Prepare the messages for the OpenAI API
    print("hhhhhhh",prompt)
    video_data = [f"data:image/jpeg;base64,{frame}" for frame in base64_frames]

    messages = [
        {
            "role": "user",
            "content": [
                {
                    "type": "video",
                    "video": video_data
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
    resultStr=""
    for response in responses:
        try:
            # 获取当前片段
            chunk = response.choices[0].delta.content
            slow_print(chunk)
            resultStr+=chunk
        except Exception as e:
            pass
    return resultStr

if __name__ == '__main__':
    # Replace with your local audio file path
    video_path = "/home/hcc/share/2025-03-13 13-05-42.mkv"
    #huishenghuiying.mp4
    #2025-03-13 13-05-42.mkv
    #prompt = "结合文字、图图像等内容，认识别角色的情感状态和情感变化。并且按照时间戳的HH:MM:SS组织时间"
    prompt="结合文字、图图像等内容，认识别角色的情感状态和情感变化。并且按照左上角红色时间戳的HH:MM:SS组织时间,时间形式为**xx:xx:xx - xx:xx:xx**"
    num_frames = 32

    # 提取视频帧
    base64_frames, timestamps = get_video_frames(video_path, num_frames)

    inference_with_openai(base64_frames, prompt)
