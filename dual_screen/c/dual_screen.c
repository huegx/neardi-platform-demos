#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int run_cmd(char *const argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        return pid; // 返回子进程 PID
    } else {
        perror("fork failed");
        return -1;
    }
}

int main() {
    int status;
    pid_t glmark_pid, cam_pid;

    // Step 1: 设置 HDMI-1 主屏，HDMI-2 在右边
    char *xrandr1[] = {"xrandr", "--output", "HDMI-1", "--primary", NULL};
    char *xrandr2[] = {"xrandr", "--output", "HDMI-2", "--right-of", "HDMI-1", "--auto", NULL};
    run_cmd(xrandr1);
    run_cmd(xrandr2);
    sleep(1);

    // Step 2: 启动 glmark2-es2
    char *glmark_cmd[] = {"glmark2-es2", "--annotate", "--size", "1280x720", "--run-forever", NULL};
    glmark_pid = run_cmd(glmark_cmd);
    sleep(2);

    // Step 3: 设置 HDMI-2 为主屏
    char *xrandr3[] = {"xrandr", "--output", "HDMI-2", "--primary", NULL};
    run_cmd(xrandr3);

    // Step 4: 启动摄像头
    char *cam_cmd[] = {
        "gst-launch-1.0",
        "v4l2src", "device=/dev/video22",
        "!", "image/jpeg,width=1280,height=720,framerate=30/1",
        "!", "mppjpegdec",
        "!", "videoconvert",
        "!", "fpsdisplaysink",
        NULL
    };
    cam_pid = run_cmd(cam_cmd);

    // Step 5: 等待 2 秒，检测摄像头是否退出
    sleep(2);
    if (waitpid(cam_pid, &status, WNOHANG) > 0) {
        printf("摄像头启动失败，改为打开 Chromium...\n");
        char *chromium_cmd[] = {"chromium", NULL};
        run_cmd(chromium_cmd);
    }

    // Step 6: 等待 glmark2 和摄像头/Chromium
    waitpid(glmark_pid, &status, 0);
    waitpid(cam_pid, &status, 0);

    return 0;
}
