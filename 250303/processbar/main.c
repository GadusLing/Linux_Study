#include "process.h"
#include <unistd.h>

//场景
double target_file_size = 1024.0;//M,目标文件的大小
double speed = 1.0; //M

void DownLoad(double size, double split)
{
    double current_total = 0.0;
    while(current_total <= size)
    {
        FlushProcess(size, current_total);// process(), 进度条不能一次刷完，要根据具体的进度来刷新
        if(current_total >= size) break;
        //用sleep来模拟下载
        usleep(10000);//代表一次下载
        current_total += split;
    }
}

int main()
{
    printf("下载中:\n");
    DownLoad(target_file_size, speed);
    printf("下载中:\n");
    DownLoad(100.0, 1.0);
    printf("下载中:\n");
    DownLoad(512.0, 2.0);
    printf("下载中:\n");
    DownLoad(1024.0, 5.0);
    return 0;
}
