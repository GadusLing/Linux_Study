#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>

int main() 
{
    int pipefd[2] = {0};  // 定义一个管道文件描述符数组，pipefd[0]表示读端，pipefd[1]表示写端
    int n = pipe(pipefd); // 创建无名管道，成功返回0，失败返回-1

    if(n == 0) {
        printf("创建管道成功: pipefd[0]: %d, pipefd[1]: %d\n", pipefd[0], pipefd[1]);
    } else {
        perror("创建管道失败");
    }

    pid_t pid = fork(); // 创建子进程

    if(pid < 0) {
        perror("fork失败");
        exit(1); // 如果fork失败，输出错误信息并退出
    } else if(pid == 0) {
        // 子进程执行的代码
        close(pipefd[0]); // 关闭管道的读端，子进程只负责写
        std::string str = "hello father, 我是子进程 ";
        std::string self = std::to_string(getpid()); // 获取子进程PID
        int cnt = 1;
        while (true) {
            // 每次循环构造完整消息
            std::string message = str + self + " " + std::to_string(cnt++) + "\n";
            write(pipefd[1], message.c_str(), message.size()); // 写入管道 要不要加\0？不用，那是语言标准，和我文件没关系
            sleep(1); // 每1秒发一次
        }

        close(pipefd[1]); // 理论上不会执行到这（死循环）
    } else {
        // 父进程执行的代码
        close(pipefd[1]); // 关闭管道的写端，父进程只负责读
        char inbuffer[1024] = {0};  // 缓冲区用于存放读取的数据

        while (true) {
            ssize_t n = read(pipefd[0], inbuffer, sizeof(inbuffer) - 1);  // 从管道读取数据
            if (n > 0) {
                inbuffer[n] = '\0';  // 确保以 '\0' 结尾，形成有效字符串
                std::cout << "client->father# " << inbuffer << std::endl;  // 打印接收到的消息
            }

            // TODO: 在此添加你需要处理的代码逻辑
            // 比如在这里可以加一些判断或其他操作来结束循环，避免死循环

            pid_t rid = waitpid(pid, nullptr, 0);  // 等待子进程结束
            (void)rid;  // 使用 (void) 去掉未使用变量的警告
        }
    }


    // int pipefd[2] = {0};
    // int n = pipe(pipefd);
    
    // if(n == 0) {
    //     printf("create pipe success: pipefd[0]: %d, pipefd[1]: %d\n", pipefd[0], pipefd[1]);
    // } else {
    //     perror("pipe failed");
    // }
    
    return 0;
}