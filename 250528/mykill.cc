#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <functional>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t id = fork();
    if (id == 0)
    {
        sleep(2);
        // int a  = 10;
        // a /= 0; // This will cause a division by zero error
        exit(1);
    }
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if (rid > 0)
    {
        printf("exit code: %d, exit signal: %d, core dump: %d\n",
               (status >> 8) & 0xFF, status & 0x7F, (status >> 7) & 0x1);
    }
    // ulimit -a 查看是否开启了 core 文件的生成

    // core-file 指令
    // 假设你有一个核心转储文件 core 和对应的可执行文件 myprogram，可以这样使用 GDB：
    // gdb myprogram
    // (gdb) core-file core
}


// void handler(int signo)
// {
//     std::cout << "我收到了一个信号：" << signo << std::endl;
//     // 故意不让进程退出
// }
//
// int main()
// {
//     //signal(SIGFPE, handler);
//     signal(SIGSEGV, handler);
//     sleep(2);

//     int *p = nullptr;
//     *p = 10; // This will cause a segmentation fault

//     // int a = 10;
//     // a /= 0; // This will cause a division by zero error
//     std::cout << "我的进程崩溃了！" << std::endl;
//     return 0;
// }

// using func_t = std::function<void()>;
// std::vector<func_t> cb;

// void FlushDisk()
// {
//     std::cout << "我是一个刷盘的操作" << std::endl;
// }

// void sched()
// {
//     std::cout << "我是一个进程调度" << std::endl;
// }

// void handler(int signo)
// {
//     for(auto &f : cb)
//     {
//         f(); // 调用所有注册的回调函数
//     }

//     (void)signo;
//     std::cout << "我是一个信号捕捉函数，我被调用了" << std::endl;
//     alarm(1);
// }

// int main()
// {
//     cb.push_back(FlushDisk);
//     cb.push_back(sched);
//     signal(SIGALRM, handler);
//     alarm(1);
//     while (true)
//     {
//         pause();
//     }
// }

// int cnt = 0;

// void handler(int signo)
// {
//     std::cout << "Received signal " << signo << "当前的计数器值为: " << cnt << std::endl;
//     exit(0);
// }

// // 未来在命令行中我们想以 ./mykill 9 1234 的输入形式来结束进程 1234
// int main(int argc, char *argv[]) // 命令行参数
// {
//     signal(14, handler);
//     alarm(1);

//     while(true)
//     {
//         cnt++;
//         //printf("%d\n", cnt++);// IO打印和单纯的cnt++效率差距极大，前者一秒是11万次计数，后者是5亿6千万
//     }

//     // // signal(14, [](int signum)
//     // //        { std::cout << "Received signal " << signum << std::endl;
//     // //          alarm(3); });
//     // alarm(3); // 设置一个闹钟，3秒后发送 SIGALRM 信号
//     // sleep(2);
//     // int n = alarm(0);// alarm(0) 取消闹钟
//     // std::cout << "剩余时间： " << n << std::endl;

//     // while (true)
//     // {
//     //     std::cout << "Running... (PID: " << getpid() << ")" << std::endl;
//     //     sleep(1); // 每秒输出一次
//     // }

//     // if (argc != 3)
//     // {
//     //     std::cerr << "Usage: " << argv[0] << " signumber pid" << std::endl;
//     //     return 1;
//     // }

//     // int signumber = std::stoi(argv[1]);
//     // pid_t target = std::stoi(argv[2]);
//     // if (kill(target, signumber) == -1)
//     // {
//     //     perror("kill");
//     //     return 2;
//     // }

//     // std::cout << "Process " << target << " killed successfully." << std::endl;

//     // signal(6, [](int signum) {
//     //     std::cout << "Received signal " << signum << std::endl;
//     // });

//     // while (true)
//     // {
//     //     sleep(1);
//     //     //raise(2);
//     //     abort(); // 发送 SIGABRT 6号 信号给自己  进程会终止
//     // }

//     return 0;
// }