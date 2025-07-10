#pragma once

#include <iostream>

// 下载任务函数
void Download() 
{
    std::cout << "我是一个下载任务" << std::endl;
}

// 打印日志任务函数
void PrintLog() 
{
    std::cout << "我是一个打印日志的任务" << std::endl;
}

// 刷新数据任务函数
void FlushData() 
{
    std::cout << "我是一个刷新数据的任务" << std::endl;
}
