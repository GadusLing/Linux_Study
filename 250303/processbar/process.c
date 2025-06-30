#include "process.h"
#include <string.h>
#include <unistd.h>

#define LENGTH 101
#define LABLE '#'

//版本1不能实际应用 只有一个进度条壳子，用来学习和说明原理，而实际的进度条需要和真实进度同步
// version 1
//void Process()
//{
//    const char *symb = "+x";
//    int s_len = strlen(symb);
//    char bar[LENGTH];
//    memset(bar, '\0', sizeof(bar));
//    int cnt = 0;
//    while(cnt<=100)
//    {
//        printf("[%-100s][%d%%][%c]\r", bar, cnt, symb[cnt%s_len]);
//        fflush(stdout);
//        bar[cnt++] = LABLE;
//
//        usleep(50000);
//    }
//    printf("\n");
//}


//version 2

void FlushProcess(double target, double current)
{
    //定义旋转光标
    const static char *sym = "|/-\\";
    static int index = 0;
    int len = strlen(sym);

    //计算进度
    double rate = (current / target)*100.0;
    //整数个进度递增，刷新一个#
    int cnt = (int)rate;
    //构建缓冲区
    char bar[LENGTH];
    memset(bar, 0, sizeof(bar));
    int i;
    for(i = 0; i < cnt; i++)
        bar[i] = LABLE;
    //刷新
    printf("[%-100s][%.1lf%%][%c]\r", bar, rate, sym[index++]);
    index %= len;
    fflush(stdout);

    if(rate >= 100.0)
        printf("\n");
}
