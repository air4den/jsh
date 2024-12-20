CC=gcc
SRC=*.c
TARGET=jsh

build:	./*.c
	$(CC) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm $(TARGET)