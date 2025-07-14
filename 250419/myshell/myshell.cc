#include "myshell.h"

// 命令行参数表，故意定义为全局
char *gargv[ARGS] = {NULL};
int gargc = 0;
char pwd[1024]; // 全局变量空间，保存当前shell进程的pwd
int lastcode = 0;

// 重定向类型
#define NONE_REDIR 0
#define OUTPUT_REDIR 1
#define APPEND_REDIR 2
#define INPUT_REDIR 3
// 重定向参数，目标文件名，重定向类型(默认无重定向)
std::string filename;
int redir_type = NONE_REDIR;

#define TrimSpace(start) do{\
    while(isspace(*start))\
    {\
        start++;\
    }\
}while(0)


// 全局变量初始化，避免数据残留影响结果
void InitGlobal()
{
    gargc = 0;
    memset(gargv, 0, sizeof(gargc));
    filename.clear();
    redir_type = NONE_REDIR;
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
    // 环境变量的变化，可能会依赖于进程，pwd需要shell自己更新环境变量的值
    //const char* pwd = getenv("PWD");
    //return pwd ? std::string(pwd) : "None";
    
    char temp[1024];
    getcwd(temp, sizeof(temp));
    //顺便更新shell自己的环境变量
    snprintf(pwd, sizeof(pwd), "PWD=%s", temp);
    putenv(pwd);

    std::string pwd_label = temp;
    const std::string pathsep = "/";
    
    // 查找最后一个路径分隔符
    auto pos = pwd_label.rfind(pathsep);
    
    // 处理未找到分隔符的情况
    if (pos == std::string::npos) 
        return "None";
    
    // 提取分隔符后的内容
    pwd_label = pwd_label.substr(pos + pathsep.size());
    
    // 处理空结果的情况（例如输入以分隔符结尾）
    return pwd_label.empty() ? "/" : pwd_label;
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

// 示例输入: "ls -a -l > log.txt"
// 效果: cmd 被截断为 "ls -a -l"，filename = "log.txt"，redir_type = OUTPUT_REDIR
void CheckRedir(char cmd[])
{
    char *start = cmd;
    char *end = cmd + strlen(cmd) - 1;

    // 从头到尾扫描命令字符串
    while (start <= end)
    {
        // 检查 >>（追加重定向）
        if (*start == '>')
        {
            if (*(start + 1) == '>')
            {
                redir_type = APPEND_REDIR;
                *start = '\0';        // 截断命令部分
                start += 2;           // 跳过 >>
                TrimSpace(start);     // 去除空格
                filename = start;     // 保存文件名
                break;
            }
            else
            {
                redir_type = OUTPUT_REDIR;
                *start = '\0';        // 截断命令部分
                start += 1;           // 跳过 >
                TrimSpace(start);     // 去除空格
                filename = start;     // 保存文件名
                break;
            }
        }

        // 检查 <（输入重定向）
        if (*start == '<')
        {
            redir_type = INPUT_REDIR;
            *start = '\0';            // 截断命令部分
            start += 1;               // 跳过 <
            TrimSpace(start);         // 去除空格
            filename = start;         // 保存文件名
            break;
        }

        start++;
    }
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

std::string GetHomePath() 
{
    return getenv("HOME") ? std::string(getenv("HOME")) : "/";
}

bool builtInCommandExec()
{
    if (gargc == 0 || gargv[0] == nullptr) 
        return false;  // 无命令可执行
    
    std::string cmd = gargv[0];
    if (cmd == "cd") 
    {
        const char *target_dir = nullptr;
        
        // 确定目标路径
        if (gargc == 1) 
            target_dir = GetHomePath().c_str();  // 无参数：切换到HOME目录
        else if (gargc == 2) 
            target_dir = gargv[1];               // 有参数：切换到指定目录
        else 
        {
            std::cerr << "cd: too many arguments\n";
            return true;  // 仍是内置命令，只是参数错误
        }

        // 执行目录切换
        if (chdir(target_dir) != 0) 
            std::cerr << "cd: " << strerror(errno) << "\n";  // 输出具体错误

        return true;  // 无论成功与否，cd命令已被处理
    }
    else if(cmd == "echo")
    {
        if(gargc == 2)
        {
            std::string args = gargv[1];
            if(args[0] == '$')
            {
                if(args[1] == '?')
                {
                    printf("lastcode:%d\n", lastcode);
                    lastcode = 0;
                    return true;
                }
                else
                {
                    const char *name = &args[1];
                    printf("%s\n", getenv(name));
                    lastcode = 0;
                    return true;
                }
            }
            else
            {
                printf("%s\n", args.c_str());
                return true;
            }
        }
    }

    // 可在此扩展其他内置命令（如exit、export等）
    return false;  // 非内置命令
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
        if (redir_type == OUTPUT_REDIR) 
        {
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(fd, 1); // 重定向标准输出到文件
        close(fd);
        } 
        else if (redir_type == INPUT_REDIR) 
        {
        int fd = open(filename.c_str(), O_RDONLY);
        dup2(fd, 0);  // 重定向标准输入到文件
        close(fd);
        } 
        else if (redir_type == APPEND_REDIR) 
        {
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
        dup2(fd, 1); // 追加输出到文件
        close(fd);
        }
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
            lastcode = WEXITSTATUS(status);
        }
    }
}
