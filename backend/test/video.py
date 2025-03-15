import os
import hashlib
import numpy as np
from PIL import Image, ImageDraw, ImageFont
import decord
from decord import VideoReader, cpu
import io
import base64
# 确保Dashscope版本不低于1.20.10
import dashscope
import time


def get_video_frames(video_path, num_frames=128, cache_dir="C:\\Users\\give4\\Downloads"):
    os.makedirs(cache_dir, exist_ok=True)  # 创建缓存目录（如果不存在）

    # 生成视频路径的MD5哈希值
    video_hash = hashlib.md5(video_path.encode('utf-8')).hexdigest()

    # 定义缓存文件路径
    frames_cache_file = os.path.join(cache_dir, f'{video_hash}_{num_frames}_frames_with_timestamps.npy')
    timestamps_cache_file = os.path.join(cache_dir, f'{video_hash}_{num_frames}_timestamps.npy')

    # 检查缓存文件是否存在
    if os.path.exists(frames_cache_file) and os.path.exists(timestamps_cache_file):
        frames = np.load(frames_cache_file)
        timestamps = np.load(timestamps_cache_file)
        return video_path, frames, timestamps

    # 读取本地视频
    vr = VideoReader(video_path, ctx=cpu(0))
    total_frames = len(vr)
    fps = vr.get_avg_fps()

    # 均匀选择需要提取的帧的索引
    indices = np.linspace(0, total_frames - 1, num=num_frames, dtype=int)
    frames = vr.get_batch(indices).asnumpy()

    # 计算时间戳
    timestamps = indices / fps  # 时间戳单位为秒

    # 在每帧上叠加时间戳
    for i in range(num_frames):
        formatted_ts = format_timestamp(timestamps[i])
        pil_img = Image.fromarray(frames[i])
        frames[i] = np.array(
            overlay_timestamp(pil_img, formatted_ts, position=(10, 10), font_size=100, color=(255, 255, 255)))

    # 缓存提取的帧和时间戳
    np.save(frames_cache_file, frames)
    np.save(timestamps_cache_file, timestamps)

    return video_path, frames, timestamps


def overlay_timestamp(image, timestamp, position=(10, 10), font_path="C:\\Windows\\Fonts\\Arial.ttf", font_size=100,
                      color=(255, 255, 255)):
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


def create_image_grid_with_timestamps(images, timestamps, num_columns=8):
    """
    创建带有时间戳的图像网格。

    :param images: 帧的 numpy 数组列表
    :param timestamps: 每帧对应的时间戳列表
    :param num_columns: 网格的列数
    :param font_path: 字体路径
    :return: 带有时间戳的图像网格
    """
    pil_images = []
    for image, ts in zip(images, timestamps):
        pil_img = Image.fromarray(image)
        formatted_ts = format_timestamp(ts)
        pil_img = overlay_timestamp(pil_img, formatted_ts, position=(10, 10), font_size=100, color=(255, 255, 255))
        pil_images.append(pil_img)

    num_rows = (len(pil_images) + num_columns - 1) // num_columns

    img_width, img_height = pil_images[0].size
    grid_width = num_columns * img_width
    grid_height = num_rows * img_height
    grid_image = Image.new('RGB', (grid_width, grid_height), color=(0, 0, 0))  # 设置网格背景为黑色

    for idx, image in enumerate(pil_images):
        row_idx = idx // num_columns
        col_idx = idx % num_columns
        position = (col_idx * img_width, row_idx * img_height)
        grid_image.paste(image, position)

    return grid_image

def convert_frames_to_base64(frames):
    base64_images = []
    for frame in frames:
        # 将帧转换为PIL图像
        pil_image = Image.fromarray(frame)
        # 将图像转换为Base64
        buffered = io.BytesIO()
        pil_image.save(buffered, format="PNG")  # 选择合适的格式
        img_str = base64.b64encode(buffered.getvalue()).decode('utf-8')
        base64_images.append(f"data:image/png;base64,{img_str}")
    return base64_images


def slow_print(text, delay=0.1):
    """逐字打印文本"""
    for char in text:
        print(char, end='', flush=True)
        time.sleep(delay)


def inference_with_dashscope(video_urls, prompt):
    # 构建消息结构
    messages = [
        {
            "role": "user",
            "content": [
                {"video": video_urls},
                {"text": prompt}
            ]
        }
    ]

    # 调用Dashscope API
    responses = dashscope.MultiModalConversation.call(
        api_key="sk-ec776f72f2594f9184141aa91bd7b78e",  # 从环境变量中获取API密钥,或者直接配置
        model='qwen-vl-max-latest',
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
            print(f"Error processing response: {e}")

if __name__ == '__main__':
    # 示例使用
    video_path = "C:\\Users\\give4\\Downloads\\28465168487-1-192.mp4"  # 替换为本地视频路径
    num_frames = 64  # 要提取的帧数

    # 提取视频帧
    video_file_path, frames, timestamps = get_video_frames(video_path, num_frames)

    # 将提取的帧转换为Base64
    video_urls = convert_frames_to_base64(frames)

    prompt = "结合文字、图图像等内容，认识别角色的情感状态和情感变化。"

    try:
        inference_with_dashscope(video_urls, prompt)
    except Exception as e:
        print("Error:", e)