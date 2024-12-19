CC=gcc
SRC=main.c
TARGET=jsh

build:	./main.c
	$(CC) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm $(TARGET)