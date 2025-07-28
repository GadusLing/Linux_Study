#include <iostream>
#include <unistd.h>

int main()
{
    while(true)
    {
        std::cout << "我是一个进程， pid： " << getpid() << std::endl;
        sleep(1);
    }

    return 0;
}