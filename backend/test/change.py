import edge_tts
import asyncio

TEXT = ""
with open('content.txt', 'r', encoding='UTF-8') as f:
    TEXT = f.read()

voice = 'zh-CN-YunxiNeural'
output = 'test.mp3'
rate = '-4%'
volume = '+0%'


async def TTS():
    tts = edge_tts.Communicate(text=TEXT, voice=voice, rate=rate, volume=volume)
    await tts.save(output)


if __name__ == '__main__':
    asyncio.run(TTS())
