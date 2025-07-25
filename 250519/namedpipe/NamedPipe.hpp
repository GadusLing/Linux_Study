#pragma once

#include "common.hpp"

const int default_fd = -1;

class NamedPipe
{
public:
    NamedPipe(const std::string &name) : _name(name), _fd(default_fd)
    {
    }
    ~NamedPipe() {};

    bool Create()
    {
        // 1.创建管道文件
        int n = mkfifo(fifoname.c_str(), mode);
        if (n == 0)
        {
            std::cout << "Named pipe created successfully." << std::endl;
        }
        else
        {
            perror("mkfifo");
            return false;
        }
        return true;
    }
    void Close()
    {
        // 2.关闭管道文件
        if (_fd == default_fd)
            return;
        else
            close(_fd);
    }
    bool OpenForRead()
    {
        _fd = open(fifoname.c_str(), O_RDONLY);
        if (_fd < 0)
        {
            perror("open");
            return false;
        }
        std::cout << "open file success" << std::endl;
        return true;
    }
    bool OpenForWrite()
    {
        _fd = open(fifoname.c_str(), O_WRONLY);
        if (_fd < 0)
        {
            perror("open");
            return false;
        }
        std::cout << "open file success" << std::endl;
        return true;
    }

    // 输入参数：const&
    // 输出参数：*
    // 输入输出参数：&
    bool Read(std::string *out)
    {
        char buffer[SIZE] = {0};
        ssize_t num = read(_fd, buffer, sizeof(buffer) - 1);
        if (num > 0)
        {
            buffer[num] = '\0';
            *out = buffer; // 将读取到的内容存入out
        }
        else if (num == 0)
        {
            return false; // EOF
        }
        else
        {
            return false; // 读取错误
        }
        return true;
    }
    void Write(const std::string &in)
    {
        write(_fd, in.c_str(), in.size());
    }
    void Remove()
    {
        int m = unlink(fifoname.c_str());
        (void)m;
    }

private:
    // std::string _path;// path + name
    std::string _name; // 管道名称
    int _fd;           // 文件描述符
};