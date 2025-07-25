#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

const std::string fifoname = "fifo";
mode_t mode = 0666;
// const int size = 128;
#define SIZE 128

#endif