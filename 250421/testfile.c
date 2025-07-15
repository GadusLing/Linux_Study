#include <stdio.h>

int main()
{
    FILE *fp = fopen("log.txt", "w");
    if(!fp)
    {
        perror("fopen");
        return 1;
    }
    fclose(fp);
    return 0;
}
