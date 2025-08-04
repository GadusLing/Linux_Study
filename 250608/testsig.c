#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

// void handler(int signo)
// {
//     pid_t rid;
//     // 使用while循环 + WNOHANG，回收所有已退出的子进程
//     while ((rid = waitpid(-1, NULL, WNOHANG)) > 0)
//     {
//         printf("%d进程，收到了信号：%d，回收子进程：%d\n", getpid(), signo, rid);
//     }
// }

int main()
{
    // signal(SIGCHLD, handler);
    signal(SIGCHLD, SIG_IGN);  // SIG_IGN Linux中忽略子进程退出信号，自动回收，但不通用
    pid_t id = fork();
    if (id == 0)
    {
        printf("我是子进程: %d\n", getpid());
        sleep(3);
        exit(0);
    }
    while(1)
    {
        printf("我是父进程：%d\n", getpid());
        sleep(1);
    }
}

// int flag = 0;

// void handler(int signo)
// {
//     flag = 1;
//     printf("改变flag: 0->1\n");
// }

// int main()
// {
//     signal(2, handler);
//     printf("进程启动: %d\n", getpid());
//     while (!flag);
//     printf("进程正常结束!\n");
//     return 0;
// }