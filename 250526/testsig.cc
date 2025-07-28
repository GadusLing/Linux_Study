#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    // 修改我自己进程的信号处理动作
    //signal(SIGINT, SIG_IGN); // 忽略2号SIGINT信号的处理，没办法ctrl + c中断了
    //signal(SIGINT, SIG_DFL);// 默认的信号处理动作 

    // 这里注意参数问题，参数类型一定是int，返回值类型是void 这里返回值lambda会自己推
    // 这里的函数参数sigon是进程收到的那个信号的编号，传几号信号，这里就是几，现在传的2，signo就是2
    signal(2, [](int signo) {
        std::cout << "进程: " << getpid() << "捕获到信号 " << signo << std::endl;
    });

    while (true)
    {
        std::cout << "我是一个进程 " << getpid() << std::endl;
        sleep(1); 
        // ctrl + c中断就是一种发信号，发的是2号信号 SIGINT
    }
    return 0;
}