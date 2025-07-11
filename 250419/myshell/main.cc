#include "myshell.h"

int main()
{
    char commandstr[SIZE]; 
    while(true)
    {
        InitGlobal();// 初始化全局变量，避免数据残留
        printCommandPrompt();// 输出命令行提示符
        if(!getCommandString(commandstr, SIZE))// 获取用户输入
            continue;
        //printf("%s\n", commandstr);
        parseCommandString(commandstr);// 解析string命令获得参数数与参数列表
        forkAndExec();// 执行命令，让子程序来执行

    }
    return 0;
}
