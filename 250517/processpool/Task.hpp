#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
// 4种任务
// task_t[4];

using task_t = std::function<void()>;


void Download()
{
    std::cout << "我是一个downlowd任务" << std::endl;
}

void MySql()
{
    std::cout << "我是一个 MySQL 任务" << std::endl;
}

void Sync()
{
    std::cout << "我是一个数据刷新同步的任务" << std::endl;
}

void Log()
{
    std::cout << "我是一个日志保存任务" << std::endl;
}

std::vector<task_t> tasks;

class Init
{
public:
    Init()
    {
        tasks.push_back(Download);
        tasks.push_back(MySql);
        tasks.push_back(Sync);
        tasks.push_back(Log);
    }
};

Init ginit;
