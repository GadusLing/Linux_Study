#ifndef __PROCESS_POOL_HPP__
#define __PROCESS_POOL_HPP__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include "Task.hpp"

const int gdefault_process_num = 5;
// typedef std::function<void (int fd)> func_t;
using callback_t = std::function<void(int fd)>;

// 先描述
class Channel
{
public:
    Channel()
    {
    }
    Channel(int fd, const std::string &name, pid_t id) : _wfd(fd), _name(name), _sub_target(id)
    {
    }
    void DebugPrint()
    {
        printf("channel name: %s, wfd: %d, target pid: %d\n", _name.c_str(), _wfd, _sub_target);
    }
    ~Channel() {}
    int Fd() { return _wfd; }
    std::string Name() { return _name; }
    pid_t Target() { return _sub_target; }
    void Close() { close(_wfd); }
    void Wait()
    {
        pid_t rid = waitpid(_sub_target, nullptr, 0);
        (void)rid;
    }

private:
    int _wfd;
    std::string _name;
    pid_t _sub_target; // 目标子进程是谁

    // int _load; //
};

class ProcessPool
{
private:
    void CtrlSubProcessHelper(int &index)
    {
        // 1. 选择一个通道(进程)
        int who = index;
        index++;
        index %= _channels.size();

        // 2. 选择一个任务，随机
        int x = rand() % tasks.size(); // [0, 3]

        // 3. 任务推送给子进程
        std::cout << "选择信道: " << _channels[who].Name() << ", subtarget : " << _channels[who].Target() << std::endl;
        write(_channels[who].Fd(), &x, sizeof(x));
        sleep(1);
    }

public:
    ProcessPool(int num = gdefault_process_num) : _processnum(num)
    {
        srand(time(nullptr) ^ getpid() ^ 0x777);
    }
    ~ProcessPool()
    {
    }
    bool InitProcessPool(callback_t cb)
    {
        for (int i = 0; i < _processnum; i++)
        {
            // 1. 创建了管道
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
                return false;

            // 2. 创建子进程
            pid_t id = fork();
            if (id < 0)
                return false;
            if (id == 0)
            {
                // 子进程除了要关闭自己的w，同时也要关闭，自己从父进程哪里继承下来的所有的之前进程w端
                //我的子进程，要关闭的，从父进程哪里继承下来的wfd都在哪里？？
                // _channels本身是被子进程继承下去的.
                // 1. 子进程不要担心，父进程会影响自己的_channels.
                // 2. fork之后，当前进程，只会看到所有的历史进程的wfd,并不受后续父进程emplace_backd的影响
                std::cout << "进程:" << getpid() << ", 关闭了: ";
                for(auto &c : _channels)
                {
                    std::cout  << c.Fd() << " ";
                    c.Close();
                }
                std::cout <<"\n";
                
                // child, read
                //  3. 关闭不需要的rw端，形成信道
                close(pipefd[1]);

                // 子进程应该干什么事情啊？？
                cb(pipefd[0]);

                exit(0);
            }

            // father, write
            close(pipefd[0]);
            std::string name = "channel-" + std::to_string(i);
            _channels.emplace_back(pipefd[1], name, id);
        }
        return true;
    }

    // 2. 控制唤醒指定的一个子进程，让该子进程完成指定任务
    // 2.1 轮询选择一个子进程(选择一个信道) -- 负载均衡
    void PollingCtrlSubProcess()
    {
        int index = 0;
        while (true)
        {
            CtrlSubProcessHelper(index);
        }
    }
    void PollingCtrlSubProcess(int count)
    {
        if (count < 0)
            return;
        int index = 0;
        while (count)
        {
            CtrlSubProcessHelper(index);
            count--;
        }
    }
    void RandomCtrlSubProcess()
    {
    }
    void LoadCtrlSubProcess()
    {
    }

    // 我们的代码，其实是有一个bug 的！
    void WaitSubProcesses()
    {
        // for(int end = _channels.size()-1; end >= 0; end--)
        // {
        //     _channels[end].Close();
        //     _channels[end].Wait();
        // }
        for (auto &c : _channels)
        {
            c.Close();
            c.Wait();
        }
        // // 1. 先让所有子进程结束
        // for (auto &c : _channels)
        // {
        //     c.Close();
        // }
        // // 2. 你在回收所有的子进程僵尸状态
        // for (auto &c : _channels)
        // {
        //     c.Wait();
        //     std::cout << "回收子进程: " << c.Target() << std::endl;
        // }
    }

private:
    // 再组织
    std::vector<Channel> _channels; // 所有信道
    int _processnum;                // 有多少个子进程
};

#endif