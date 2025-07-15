#include "mystdio.h"

int main()
{
    // 以写入模式("w")打开文件"log.txt"，返回文件指针
    MyFILE *fp = my_fopen("log.txt", "w");
    if(fp == NULL)  // 检查文件是否成功打开
    {
        return 1;   // 打开失败，返回错误码1
    }

    const char *s = "hello myfile \n";  // 要写入的字符串
    int cnt = 20;                       // 写入次数计数器
    
    // 循环20次，每次写入字符串s
    while(cnt--)
    {
        my_fwrite(s, strlen(s), fp);    // 调用自定义的写入函数
    }
    
    my_fclose(fp);  // 关闭文件
    
    return 0;       // 程序正常结束
}
