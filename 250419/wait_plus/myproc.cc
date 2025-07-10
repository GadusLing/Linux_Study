#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include "Task.hpp"
#include "Tool.hpp"


int main()
{
    Tool tool;
    tool.PushFunc(Download);
    tool.PushFunc(PrintLog);
    tool.PushFunc(FlushData);

    pid_t id = fork();
    
    if (id == 0) 
    {
        // 子进程
        int cnt = 10;
        while (cnt--) 
        {
            printf("子进程在运行：%d\n", cnt);
            sleep(1);
        }
        exit(0);
    }
    
    // 父进程等待子进程（非阻塞方式）
    while(1)
    {
        pid_t rid = waitpid(id, NULL, WNOHANG);
        if (rid == id) 
        {
            printf("wait child success\n");
            break;
        } 
        else if (rid == 0) 
        {
            printf("child not quit!\n");
            // 父进程等待过程很闲，那做做其他事儿吧
            tool.Execute();
            sleep(1);
        } 
        else if (rid < 0) 
        {
            printf("wait error!\n");
            break;
        }
    }
    
    return 0;
}
