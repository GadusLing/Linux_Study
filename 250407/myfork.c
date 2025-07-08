#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        //子进程
        int cnt = 5;
        while(cnt--)
        {
            printf("我是子进程, pid: %d\n", getpid());
            sleep(1);
        }
        printf("子进程退出了\n");
        exit(1);
    }
    //父进程
    while(1)
    {
        printf("我是父进程, pid: %d\n", getpid());
        sleep(1);
    }

}
