BIN=count
CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
LFLAGS=-o
CFLAGS=-c
RM=rm -f

$(BIN):$(OBJ)
	@$(CC) $(LFLAGS) $@ $^
	@echo "链接$^ 成为 $@"
%.o:%.c
	@$(CC) $(CFLAGS) $<
	@echo "编译$< 成为 $@"

.PHONY:clean
clean:
	@$(RM) $(BIN) $(OBJ)
	@echo "清理工程完毕"

.PHONY:print
print:
	@echo $(SRC)
	@echo "---------------------------------"
	@echo $(OBJ)




#BIN=code
#CC=gcc
#SRC=code.c
#OBJ=code.o
#LFLAGS=-o
#CFLAGS=-c
#RM=rm -f
#
#$(BIN):$(OBJ)
#	$(CC) $(LFLAGS) $@ $^
#$(OBJ):$(SRC)
#	$(CC) $(CFLAGS) $(SRC)
#.PHONY:clean
#clean:
#	$(RM) $(BIN) $(OBJ)
#
#
#.PHONY:print
#print:
#	@echo $(BIN)
#	@echo $(CC)
#	@echo $(SRC)
#	@echo $(FLAGS)
#	@echo $(RM)



#BIN=code
#CC=g++
#SRC=code.c
#FLAGS=-o
#RM=rm -f
#
#$(BIN):$(SRC)
#	$(CC) $(FLAGS) $(BIN) $(SRC)
#.PHONY:clean
#clean:
#	$(RM) $(BIN)
#
#
#.PHONY:print
#print:
#	@echo $(BIN)
#	@echo $(CC)
#	@echo $(SRC)
#	@echo $(FLAGS)
#	@echo $(RM)







#code:code.o
#	@gcc code.o -o code
#code.o:code.c
#	@gcc -c code.c
#.PHONY:clean
#clean:
#	@rm -f code code.o



#code:code.o
#	gcc code.o -o code
#
#code.o:code.s
#	gcc -c code.s -o code.o
#
#code.s:code.i
#	gcc -S code.i -o code.s
#
#code.i:code.c
#	gcc -E code.c -o code.i
#
#.PHONY:clean
#clean:
#	rm -f code code.i code.s code.o
