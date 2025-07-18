#include "mystdio.h"

// 文件打开模式处理函数
// 参数：filename-文件名, mode-模式字符串
MyFILE *my_fopen(const char *filename, const char *mode) 
{
    int fd = -1;//  open返回值：成功返回文件描述符fd，失败返回-1

    // 模式解析与处理
    if (strcmp(mode, "r") == 0) 
    {
        // "r"：只读模式，文件必须存在
        fd = open(filename, O_RDONLY);
    }
    else if (strcmp(mode, "r+") == 0) 
    {
        // "r+"：读写模式，文件必须存在
        fd = open(filename, O_RDWR);
    }
    else if (strcmp(mode, "w") == 0) 
    {
        // "w"：只写模式，创建或清空文件
        fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, UMASK);
    }
    else if (strcmp(mode, "w+") == 0) 
    {
        // "w+"：读写模式，创建或清空文件
        fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, UMASK);
    }
    else if (strcmp(mode, "a") == 0) 
    {
        // "a"：追加写入模式，创建或追加到文件末尾
        fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, UMASK);
    }
    else if (strcmp(mode, "a+") == 0) 
    {
        // "a+"：读写追加模式，创建或追加到文件末尾
        fd = open(filename, O_CREAT | O_RDWR | O_APPEND, UMASK);
    }
    else if (strcmp(mode, "b") == 0 || strcmp(mode, "t") == 0) 
    {
        // "b"或"t"：二进制/文本模式（在Linux中无区别，直接兼容处理）
        // 实际模式需要组合使用（如"rb"、"wb+"等），此处需结合前驱字符判断
        // 示例简化处理：若单独传入"b"或"t"，默认按"r"模式
        fd = open(filename, O_RDONLY);
    }
    else 
    {
        // 非法模式：打印错误并返回失败
        fprintf(stderr, "Error: Invalid file mode '%s'. ", mode);
        fprintf(stderr, "Supported modes: r, r+, w, w+, a, a+, [b, t]\n");
        return NULL;
    }

    // 统一检查文件打开结果
    if (fd < 0) 
    {
        perror("File open failed");
        return NULL;
    }
    MyFILE *fp = (MyFILE*)malloc(sizeof(MyFILE));
    if (!fp) return NULL;
    fp->fileno = fd;
    fp->flag = FLUSH_LINE;  // 默认行缓冲
    fp->curr = 0;
    fp->cap = SIZE;
    fp->outbuffer[0] = 0;

    return fp;
}

void my_fclose(MyFILE *fp)
{
    if (!fp) return;
    
    my_fflush(fp); // 用户区 刷到 C缓存区
    close(fp->fileno);// C缓存区 刷到 内核
    free(fp);
}

static void my_fflush_core(MyFILE *fp, int force)
{   
    if(fp->curr <= 0) return;
    if(force == FORCE)// 强制刷新
    {
        write(fp->fileno, fp->outbuffer, fp->curr);
        fp->curr = 0;
    }
    else // 常规刷新
    {
        // 情况1: 行缓冲模式(FLUSH_LINE)且遇到换行符
        if((fp->flag & FLUSH_LINE) && fp->outbuffer[fp->curr-1] == '\n') 
        {
            // 将缓冲区内容写入文件
            write(fp->fileno, fp->outbuffer, fp->curr);
            fp->curr = 0; // 重置缓冲区位置
        }
        // 情况2: 全缓冲模式(FLUSH_FULL)且缓冲区已满
        else if((fp->flag & FLUSH_FULL) && fp->curr == fp->cap) 
        {
            // 将缓冲区内容写入文件
            write(fp->fileno, fp->outbuffer, fp->curr);
            fp->curr = 0; // 重置缓冲区位置
        }
        // 情况3: 不需要刷新
        else 
        {
            // 无缓冲模式
            //write(fp->fileno, fp->outbuffer, fp->curr);
        }
    }
}

/**
 * 自定义文件写入函数，模拟标准库的fwrite
 * @param s 要写入的数据源缓冲区指针
 * @param size 要写入的字节数
 * @param fp 自定义文件结构体指针
 * @return 实际写入的字节数(总是等于size)
 */
int my_fwrite(const char *s, int size, MyFILE *fp)
{
    // 1. 将数据复制到输出缓冲区
    memcpy(fp->outbuffer + fp->curr, s, size); // 将数据从s复制到输出缓冲区的当前写入位置
    fp->curr += size; // 更新缓冲区当前写入位置

    // 2. 检查是否需要刷新缓冲区(将缓冲数据写入实际文件)
    my_fflush_core(fp, NORMAL); 
    
    // 3. 返回实际写入的字节数
    return size;
}

void my_fflush(MyFILE *fp)// 强制调用刷新，暴露给外部的接口，封装了一层
{
    my_fflush_core(fp, FORCE);
}


