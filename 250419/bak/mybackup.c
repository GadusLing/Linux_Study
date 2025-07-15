#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int garray[100];

pid_t backup(const char *filename)
{
    pid_t id = fork();
    if(id == 0)
    {
        // 子进程完成数据的备份
        FILE *fp = fopen(filename, "w");
        for(int i = 0; i < 100; i++)
        {
            fprintf(fp, "%d ", garray[i]);
        }
        fclose(fp);
        exit(0);
    }
    return id;
}


int main()
{
    srand(time(nullptr) ^ getpid());
    // 父进程未来的数据
    for(int i = 0; i < 100; i++)
    {
        garray[i] = rand() % 10;
    }
    // 要求父进程把自己每一轮的数据都进行保存
    pid_t sub1 = backup("log1.txt");

    for(int i = 0; i < 100; i++)
    {
        garray[i] = rand() % 10;
    }
    pid_t sub2 = backup("log2.txt");

    for(int i = 0; i < 100; i++)
    {
        garray[i] = rand() % 10;
    }
    pid_t sub3 = backup("log3.txt");

    waitpid(sub1, NULL, 0);
    waitpid(sub2, NULL, 0);
    waitpid(sub3, NULL, 0);
    
    return 0;
}
