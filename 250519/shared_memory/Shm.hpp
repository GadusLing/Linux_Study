#ifndef __SHM_HPP__
#define __SHM_HPP__

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

std::string gpathname = "."; // 共享内存文件路径
int gproj_id = 0x66;         // 共享内存项目ID
int gdefault_size = 4096;    // 默认共享内存大小

// 共享内存结构：把读写指针也放进共享区
struct ShmBlock {
    size_t windex;
    size_t rindex;
    char buffer[1]; // 占位符 实际 buffer 大小是 gdefault_size - sizeof(windex)-sizeof(rindex)
};

// 共享内存
class SharedMemory
{
public:
    SharedMemory(int size = gdefault_size)
        : _size(size), _key(0), _shmid(-1), _start_addr(nullptr)
    {
    }
    ~SharedMemory() {}

    // 创建共享内存
    bool Create()
    {
        return CreateHelper(IPC_CREAT | IPC_EXCL | 0666);
    }

    // 获取共享内存，供客户端使用，单独采用 IPC_CREAT的方式，如果共享内存不存在则创建，存在则获取
    bool Get()
    {
        return CreateHelper(IPC_CREAT);
    }

    // 挂接共享内存到调用该函数进程的虚拟地址空间
    bool Attach()
    {
        _start_addr = shmat(_shmid, nullptr, 0); // 保存起始地址
        if (_start_addr == (void *)-1)
        {
            perror("shmat");
            return false;
        }
        std::cout << "将指定的共享内存空间挂接到进程的虚拟地址空间" << std::endl;
        return true;
    }

    // 有挂载就有断联，断开进程虚拟地址空间与共享内存的映射，释放进程中的空间，断开页表映射，修改关联计数nattch
    // 这样移除自身关联后，其他进程仍然可以正常访问共享内存，而进程自身的空间也空出来了
    // 而并非是强制的直接移除共享内存本身
    bool Detach()
    {
        if (shmdt(_start_addr) == -1)
        {
            perror("shmdt");
            return false;
        }
        _start_addr = nullptr; // 清除起始地址
        std::cout << "将指定的共享内存空间从进程的虚拟地址空间移除" << std::endl;
        return true;
    }

    // 正式通信，向共享内存中添加数据，这里用模板来写
    template <typename T>
    bool AddData(const T& data) {
        if (!IsAttached()) return false;

        auto blk = (ShmBlock*)_start_addr;
        size_t dataSize = sizeof(T);
        if (!HasSpace(dataSize)) {
            std::cerr << "共享内存空间不足，无法添加数据" << std::endl;
            return false;
        }
        memcpy(blk->buffer + blk->windex, &data, dataSize);
        blk->windex = (blk->windex + dataSize) % BufferSize();
        return true;
    }

    // 同样的也可以增加string
    bool AddString(const std::string& str) {
        if (!IsAttached()) return false;

        auto blk = (ShmBlock*)_start_addr;
        size_t len = str.size() + 1;
        if (!HasSpace(len)) {
            std::cerr << "共享内存空间不足，无法添加字符串" << std::endl;
            return false;
        }
        memcpy(blk->buffer + blk->windex, str.c_str(), len);
        blk->windex = (blk->windex + len) % BufferSize();
        return true;
    }

    // 能输入信息就要能读取信息
    bool PopChar(char* ch) {
        if (!IsAttached()) return false;

        auto blk = (ShmBlock*)_start_addr;
        if (blk->rindex == blk->windex) {
            std::cerr << "没有新数据可读" << std::endl;
            return false;
        }
        *ch = blk->buffer[blk->rindex];
        blk->rindex = (blk->rindex + 1) % BufferSize();
        return true;
    }

    // 移除共享内存
    bool RemoveShm()
    {
        if (shmctl(_shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl");
            return false;
        }
        _shmid = -1; // 清除共享内存ID
        std::cout << "shm已被移除" << std::endl;
        return true;
    }

    // 获取共享内存相关的属性
    void PrintAttr()
    {
        struct shmid_ds ds;
        if(shmctl(_shmid, IPC_STAT, &ds) == -1)
        {
            perror("shmctl");
            return;
        }
        printf("key: 0x%x\n", ds.shm_perm.__key);
        printf("size: %zu\n", ds.shm_segsz);
        printf("atime: %lu\n", ds.shm_atime);
        printf("nattch: %lu\n", ds.shm_nattch);

    }

private:
    key_t _key;
    int _size;
    int _shmid;
    void *_start_addr;

    // 创建共享内存的辅助函数，根据fllags参数来决定是创建全新的共享内存还是获取已存在的共享内存
    bool CreateHelper(int flags)
    {
        // 生成共享内存的key
        _key = ftok(gpathname.c_str(), gproj_id);
        if (_key == -1)
        {
            perror("ftok");
            return false;
        }
        printf("形成键值成功：0x%x\n", _key);

        _shmid = shmget(_key, _size, flags); // 创建全新的shm
        if (_shmid == -1)
        {
            perror("shmget");
            return false;
        }
        printf("shmid：%d\n", _shmid); // 打印共享内存ID
        return true;
    }

    // 检查共享内存是否连接了
    bool IsAttached() const {
        if (_start_addr == nullptr) {
            std::cerr << "共享内存未挂接，请先调用Attach()" << std::endl;
            return false;
        }
        return true;
    }

    // 获取共享内存的缓冲区大小
    size_t BufferSize() const {
        return _size - sizeof(size_t) * 2;
    }

    // 检查共享内存是否有足够的空间来存储数据
    bool HasSpace(size_t dataSize) const {
        auto blk = (ShmBlock*)_start_addr;
        // 简化版：不考虑满/空细节，只判断容量足够
        return (blk->windex + dataSize) < BufferSize();
    }

};

#endif