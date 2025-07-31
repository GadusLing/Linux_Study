#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <functional>
#include <sys/wait.h>

// PrintPending 函数声明
void PrintPending(sigset_t &pending);
void handler(int signo)
{
    std::cout << "我获取到了:" << signo << "信号" << std::endl;
    sigset_t pending;
    sigemptyset(&pending);
    // 2.1获取当前进程的pending信号集
    sigpending(&pending);

    // 2.2不断打印所有的pending信号集中的信号
    std::cout << "#######################################" << std::endl;
    PrintPending(pending);
    std::cout << "#######################################" << std::endl;
}

void PrintPending(sigset_t &pending)
{
    // 右->左， 低->高， 0000 0000
    std::cout << "[pid:" << getpid() << "]" << "sigpending list:";
    for (int signo = NSIG; signo > 0; --signo) // NSIG is the number of signals
    {
        if (sigismember(&pending, signo))
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << "\r\n";
}

int main()
{
    // 设置2号信号的处理动作，不要让它终止
    signal(SIGINT, handler);

    // 1.屏蔽2号信号
    // 1.1用户层面，设置位图
    sigset_t block, oblock;
    sigemptyset(&block);
    sigemptyset(&oblock);

    sigaddset(&block, SIGINT); // 这里的时候，我们有没有设置当前进程的信号屏蔽字？没有！！！！

    // 1.3:屏蔽所有信号
    for (int i = 1; i <= 31; i++)
        sigaddset(&block, i);

    // 1.2设置内核的信号屏蔽字
    sigprocmask(SIG_SETMASK, &block, &oblock);

    int cnt = 15;
    while (true)
    {
        while (true)
        {
            sigset_t pending;
            sigemptyset(&pending);
            // 2.1获取当前进程的pending信号集
            sigpending(&pending);

            // 2.2不断打印所有的pending信号集中的信号
            PrintPending(pending);

            cnt--;
            if (cnt == 0)
            {
                // 解除对2号的屏蔽
                std::cout << "[pid:" << getpid() << "]" << "解除对2号信号的屏蔽了\r\n";
                sigprocmask(SIG_SETMASK, &oblock, nullptr);
            }

            sleep(1);
        }
    }
}

// 扩展理解：
// ·1.问题：屏蔽所有信号？？
//  9号信号不可屏蔽，9号信号是SIGKILL，不能被阻塞或忽略。

// 2．问题：如果解除对2号的屏蔽，我也要看到1->0

// 3.问题：2号信号被递达，pending1->0，递达前变化，还是递达后变化?
//   答案：递达前变化
