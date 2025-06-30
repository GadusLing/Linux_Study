#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("hello, world!");
    fflush(stdout);
    sleep(2);
    return 0;
}
