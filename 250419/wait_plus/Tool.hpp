#pragma once

#include <iostream>
#include <vector>
#include <functional>

using func_t = std::function<void()>;
// 等价于: typedef std::function<void()> func_t;

class Tool 
{
public:
    Tool() = default;  // 默认构造函数
    
    void PushFunc(func_t f) 
    {
        _funcs.push_back(f);
    }
    
    void Execute() 
    {
        for (auto& f : _funcs) 
        {
            f();
        }
    }
    
    ~Tool() = default;  // 默认析构函数

private:
    std::vector<func_t> _funcs;  // 方法集合
};
