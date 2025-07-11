#include "myshell.h"

// 命令行参数表，故意定义为全局
char *gargv[ARGS] = {NULL};
int gargc = 0;

// 全局变量初始化，避免数据残留影响结果
void InitGlobal()
{
    gargc = 0;
    memset(gargv, 0, sizeof(gargc));
}


static std::string getUserName() 
{
    if(const char* user = getenv("USER"))
        return user;
    return "None";
}

static std::string getHostName() 
{
    const char* hostname = getenv("HOSTNAME");
    return hostname ? std::string(hostname) : "None";
}

static std::string getPwd() 
{
    const char* pwd = getenv("PWD");
    return pwd ? std::string(pwd) : "None";
}

void printCommandPrompt() 
{
    std::string user = getUserName();
    std::string hostname = getHostName();

    std::string pwd = getPwd();
    printf("[%s@%s %s]# ", user.c_str(), hostname.c_str(), pwd.c_str());
}

bool getCommandString(char cmd_str_buff[], int len)
{
    // 检查缓冲区是否有效
    if (cmd_str_buff == NULL || len <= 0) 
        return false;
    
    // 使用 fgets 安全读取输入（防止缓冲区溢出）
    if (fgets(cmd_str_buff, len, stdin) == NULL) 
        return false;  // 读取错误或 EOF

    // 移除末尾的换行符（如果有）
    size_t input_len = strlen(cmd_str_buff);
    if (input_len > 0 && cmd_str_buff[input_len - 1] == '\n') 
       cmd_str_buff[input_len - 1] = '\0';

    return strlen(cmd_str_buff) == 0 ? false : true;
}

bool parseCommandString(char cmd[])
{
    if (cmd == NULL) return false;  // 如果输入字符串为空，直接返回失败

#define SEP " "  // 定义分隔符为 " "

    // 使用 strtok 分割字符串，第一个参数
    gargv[gargc++] = strtok(cmd, SEP);

    // 继续分割剩余部分，直到返回 NULL
    while ((bool)(gargv[gargc++] = strtok(NULL, SEP)));

    // 由于 while 循环会多递增一次 gangc，所以需要回退
    gargc--;


//#define DEBUG  // 定义 DEBUG 宏，启用调试输出

#ifdef DEBUG  // 如果 DEBUG 已定义，则编译以下代码
    // 打印参数个数 (gargc)
    printf("gargc: %d\n", gargc);
    printf("------------------\n");

    // 方式1：通过 gargc 遍历参数数组 (gargv)
    for (int i = 0; i < gargc; i++) 
        printf("gargv[%d]: %s\n", i, gargv[i]);
    printf("------------------\n");

    // 方式2：通过 NULL 结尾遍历参数数组 (gargv)
    for (int i = 0; gargv[i] != NULL; i++)
        printf("gargv[%d]: %s\n", i, gargv[i]);
#endif	

    return true;  // 解析成功<Paste>
}

void forkAndExec()
{
    pid_t id = fork();  // 创建子进程
    if (id < 0) 
    {
        // fork 失败
        perror("fork");  //  perror，打印系统错误信息
        return;
    } 
    else if (id == 0) 
    {
        // 子进程
        execvp(gargv[0], gargv);  // 执行命令（gargv[0]是程序名，gargv是参数数组）
        perror("execvp");         // 如果execvp失败，打印错误（例如命令不存在）
        _exit(127);               // 用_exit退出，避免刷新父进程的I/O缓冲区
    } 
    else 
    {
        // 父进程
        int status;
        pid_t rid = waitpid(id, &status, 0);  // 阻塞等待子进程结束
        if (rid > 0) 
        {
            // 检查子进程退出状态
            //if (WIFEXITED(status)) 
            //{
            //    printf("Command exited with status %d\n", WEXITSTATUS(status));
            //} 
            //else if (WIFSIGNALED(status)) 
            //{
            //    printf("Command killed by signal %d\n", WTERMSIG(status));
            //}
        }
    }
}
