CC = gcc
CFLAGS = -Wall -Wextra
TARGET = myexec
SRC = myexec.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)
