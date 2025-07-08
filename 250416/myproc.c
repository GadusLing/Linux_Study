#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int print()
{
    printf("hello print()\n");
    exit(10);
}

int main()
{
    printf("我的进程开始运行了\n");
    sleep(1);
    print();
    printf("我的进程运行结束了\n");

    return 4;
}
