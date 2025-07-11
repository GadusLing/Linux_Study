#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 1024
#define ARGS 64

// 函数声明
int Debug();
void InitGlobal();
void printCommandPrompt();
bool getCommandString(char cmd_str_buff[], int len = SIZE);
bool parseCommandString(char cmd[]);
void forkAndExec();

#endif
