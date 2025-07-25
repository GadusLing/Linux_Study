#include "Shm.hpp"

int main()
{
    SharedMemory shm;
    shm.Get();
    // sleep(2);

    shm.Attach();
    // sleep(2);

    // 使用
    char c = 'A';
    for(; c <= 'Z'; c++)
    {
        shm.AddData(c);
        printf("Client: %c\n", c);
        sleep(1);
    }

    shm.Detach();
    // sleep(2);

    // shm.RemoveShm();
    // sleep(2);

    return 0;
}