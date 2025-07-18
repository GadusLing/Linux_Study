# 生成动态库
libmyc.so: mymath.o mystdio.o
	gcc -shared -o $@ $^

# 编译 .c 为 .o
%.o: %.c
	gcc -fPIC -c $<

# 打包动态库和头文件
.PHONY: output
output:
	mkdir mylibc
	mkdir -p mylibc/include mylibc/lib
	cp *.h mylibc/include
	cp *.so mylibc/lib
	tar czf mylibc.tgz mylibc

# 清理
.PHONY: clean
clean:
	rm -rf *.so* *.o mylibc *.tgz
