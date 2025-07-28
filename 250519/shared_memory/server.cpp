#include "Shm.hpp"

int main()
{
    SharedMemory shm;
    shm.Create();
    // sleep(2);

    shm.Attach();
    // sleep(2);

    shm.PrintAttr();
    sleep(3);

    // 使用
    while(true)
    {
        char c;
        shm.PopChar(&c);
        printf("server received: %c\n", c);

        sleep(1);
    }

    shm.Detach();
    // sleep(2);

    shm.RemoveShm();
    // sleep(2);

    return 0;
}