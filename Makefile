CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = lab26
SRC = lab26.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
