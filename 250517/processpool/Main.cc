#include "ProcessPool.hpp"

int main()
{
    // 1. 创建进程池
    ProcessPool pp(5);

    // 2. 初始化进程池
    pp.InitProcessPool([](int fd){
        while(true)
        {
            int code = 0;
            //std::cout << "子进程阻塞: " << getpid() << std::endl;
            ssize_t n = read(fd, &code, sizeof(code));
            if(n == sizeof(code)) // 任务码
            {
                std::cout << "子进程被唤醒: " << getpid() << std::endl;
                if(code >= 0 && code < tasks.size())
                {
                    tasks[code]();
                }
                else
                {
                    std::cerr << "父进程给我的任务码是不对的: " << code << std::endl;
                }
            }
            else if(n == 0)
            {
                std::cout << "子进程应该退出了: " << getpid() << std::endl;
                break;
            }
            else
            {
                std::cerr << "read fd: " << fd << ", error" << std::endl;
                break;
            }
        } 
    });

    // 3. 控制进程池
    pp.PollingCtrlSubProcess(10);

    // 4. 结束线程池
    pp.WaitSubProcesses();
    std::cout << "父进程控制子进程完成，父进程结束" << std::endl;

    return 0;
}