#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char *my = "hello=123456789";// 尝试打印系统环境变量的同时又能自己新增环境变量

int main(int argc, char *argv[], char **env) 
{
    extern char **environ;
    pid_t id = fork();
    
    if (id == 0) 
    {  // 子进程
        //sleep(2);
        printf("我是一个子进程：%d\n", getpid());
        //sleep(1);
        

        // 定义命令行参数数组（必须以NULL结尾）
        char *const myargv[] = {"mycmd", "arg1", "arg2", NULL};
        // 定义环境变量数组（必须以NULL结尾）
        //char *const myenv[] = {"PATH=/usr/bin", "WORLD=1", NULL};
        // 执行进程替换
        
        putenv(my);
        execvpe("./mycmd", myargv, environ);

        //execlp("ls", "ls", "-a", "-l", NULL);

        //execl("./mycmd", "mycmd", NULL);// 子进程既然能执行系统命令，那也可以执行我们自己的命令
        
        //execl("/usr/bin/python3", "python3", "mypy.py", NULL);//也可以执行python的语句

        //execl("/usr/bin/bash", "bash", "test.sh", NULL);//脚本语言也是可以的
        // 尝试执行ls命令
        //execl("/usr/bin/ls", "ls", "-a", "-l", "-n", NULL);
        //char *const myargv[] = {
        //  (char*)"ls",
        //  (char*)"-a",
        //  (char*)"-i",
        //  (char*)"-n",
        //  (char*)"-l",
        //  NULL
        //};

        //execvp(myargv[0], myargv);

        //char *const myargv[] = {(char*)"pwd", NULL};
        //char **myargv = &argv[1];
        //execv(myargv[0], myargv);

        // 只有execl失败才会执行到这里
        printf("程序替换失败!\n");
        exit(1);
    }
    else if (id > 0) 
    {  
        // 父进程
        pid_t rid = waitpid(id, NULL, 0);
        if (rid == id) 
        {
            printf("wait %d success\n", id);
        }
        else 
        {
            printf("wait failed\n");
        }
    }
    else 
    {  
        // fork失败
        perror("fork failed");
        exit(1);
    }

    return 10;
}
