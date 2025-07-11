XX = g++  # 使用标准 C++ 编译器变量名
CXXFLAGS = -Wall -Wextra
TARGET = myshell
SRCS = main.cc myshell.cc
OBJS = $(SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lstdc++  # 添加 -lstdc++

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
