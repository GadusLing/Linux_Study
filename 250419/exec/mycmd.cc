#include <iostream>
#include <cstdio>
#include <unistd.h>

//int main() 
//{
//    std::cout << "hello C++ pragma" << std::endl;  // 输出字符串并换行
//    return 0;  // 隐式返回0（可省略）
//}

int main(int argc, char *argv[], char *env[]) 
{
    int i = 0;
    
    // 打印所有命令行参数
    for (i = 0; i < argc; i++) 
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    
    // 打印所有环境变量
    for (i = 0; env[i]; i++) 
    {    // env[i]为NULL时终止循环
        printf("env[%d]: %s\n", i, env[i]);
    }
    printf("我是程序替换后，我的pid: %d\n\n", getpid());   
    return 0;
}
