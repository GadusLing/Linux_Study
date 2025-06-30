#include <stdio.h>

#define MAX 100
#define VERSION

int main()
{
    printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    //printf("hello, world! hello, LDW! %d\n", MAX);
    printf("hello, world! hello, LDW! %d\n", MAX);

#ifdef VERSION
    printf("hello version new!\n");
#else
    printf("hello default version\n");
#endif
    return 0;
}

