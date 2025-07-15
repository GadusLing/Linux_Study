#include <unistd.h>   // 
#include <stdlib.h>   // 提供malloc/free等函数
#include <string.h>   // 提供strcmp等字符串操作
#include <sys/types.h> // 提供系统类型定义（如mode_t）
#include <sys/stat.h>  // 提供文件状态操作和权限常量
#include <fcntl.h>     // 提供文件控制选项（如O_RDONLY）
#include <unistd.h>    // 提供系统调用（如open/close）
#include <stdio.h>     // 提供错误输出（perror)


#ifndef __MYSTDIO_H__
#define __MYSTDIO_H__

// 缓冲区刷新模式
#define FLUSH_NONE 1  // 不主动刷新
#define FLUSH_LINE 2  // 行缓冲（遇换行符刷新）
#define FLUSH_FULL 4  // 全缓冲（缓冲区满时刷新）

#define SIZE 4096     // 缓冲区大小
#define UMASK 0666    // 权限掩码

#define FORCE 1    // 强制刷新
#define NORMAL 2    // 常规刷新



// 自定义文件流结构体（模仿FILE）
typedef struct _MY_IO_FILE
{
    int fileno;       // 文件描述符
    int flag;         // 刷新模式
    char outbuffer[SIZE]; // 输出缓冲区
    int curr;         // 当前缓冲区位置
    int cap;          // 缓冲区容量
} MyFILE;

// 函数声明
MyFILE *my_fopen(const char *filename, const char *mode);
void my_fclose(MyFILE *fp);
int my_fwrite(const char *s, int size, MyFILE *fp);
void my_fflush(MyFILE *fp);


#endif
