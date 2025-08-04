#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
    std::cout << "获取到一个信号：" << signo << std::endl;
    while (true)
    {
        sigset_t pending;
        sigemptyset(&pending);
        sigpending(&pending);
        for (int i = 31; i > 0; i--)
        {
            if (sigismember(&pending, i))
                std::cout << "1";
            else
                std::cout << "0";
        }
        std::cout << std::endl;
        sleep(1);
    }
}

int main()
{
    struct sigaction act, oact;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&(act.sa_mask)); // 我们现在有没有设置到内核？没有
    // 谈sigaction主要就是要谈这个sa_mask
    sigaddset(&(act.sa_mask), 3);
    sigaddset(&(act.sa_mask), 4);
    sigaddset(&(act.sa_mask), 5);
    sigaddset(&(act.sa_mask), 6);

    sigaction(SIGINT, &act, &oact); // 将2号信号的捕捉方法，设置到内核中！

    while (true)
    {
        std::cout << "我是一个进程：" << getpid() << std::endl;
        sleep(1);
    }

    return 0;
}