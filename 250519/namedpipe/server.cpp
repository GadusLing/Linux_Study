#include "NamedPipe.hpp"

int main()
{
    NamedPipe named_pipe(fifoname);
    named_pipe.Create();
    named_pipe.OpenForRead();
    // 2.打开管道文件，和普通文件一样访问
    // 运行时会发现Named pipe created successfully但没有open file success
    // 这是因为命名管道的操作特点，打开一端，而另一端没有打开时，open会阻塞

    // 3.通信
    std::string message;
    while (true)
    {
        bool res = named_pipe.Read(&message);
        if (!res)
        {
            break;
        }
        std::cout << "client say: " << message << std::endl;
    }

    // 4.关闭描述符，管道文件,归还资源
    named_pipe.Close();
    named_pipe.Remove();

    return 0;
}