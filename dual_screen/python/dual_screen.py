#!/usr/bin/env python3
import subprocess
import time

# --- 步骤 1: 左屏 HDMI-1 为主屏，启动 glmark2-es2 ---
subprocess.run(["xrandr", "--output", "HDMI-1", "--primary"])
subprocess.run(["xrandr", "--output", "HDMI-2", "--right-of", "HDMI-1", "--auto"])

# 启动 glmark2-es2
# glmark_proc = subprocess.Popen(["glmark2-es2"])
glmark_proc = glmark_proc = subprocess.Popen(["glmark2-es2", "--annotate", "--size", "1280x720", "--run-forever"])
time.sleep(2)  # 等待窗口创建

# --- 步骤 2: HDMI-2 为主屏，启动摄像头 ---
subprocess.run(["xrandr", "--output", "HDMI-2", "--primary"])

# 启动摄像头播放（硬解）
cam_cmd = [
    "gst-launch-1.0",
    "v4l2src", "device=/dev/video22",
    "!", "image/jpeg,width=1280,height=720,framerate=30/1",
    "!", "mppjpegdec",
    "!", "videoconvert",
    "!", "fpsdisplaysink"
]
cam_proc = subprocess.Popen(cam_cmd)

# 等待 2 秒看摄像头是否成功运行
time.sleep(2)
if cam_proc.poll() is not None:  # 进程已经退出，说明失败
    print("摄像头启动失败，改为打开 Chromium...")
    subprocess.Popen(["chromium"])

# 等待两个进程运行
try:
    glmark_proc.wait()
    cam_proc.wait()
except KeyboardInterrupt:
    glmark_proc.terminate()
    cam_proc.terminate()
