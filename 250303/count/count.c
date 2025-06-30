#include <stdio.h>
#include <unistd.h>
int main()
{
    int cnt = 100;
    while(cnt)
    {
        printf("%-3d\r", cnt);
        fflush(stdout);
        cnt--;
        sleep(1);
    }
    printf("\n");

    return 0;
}
