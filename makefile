CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = test
SRC = src/cria_func.c tests/test.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: all
	./$(TARGET)

clean:
	rm -f $(TARGET)