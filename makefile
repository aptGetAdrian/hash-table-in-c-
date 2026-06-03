# For a single C file
CC = gcc
CFLAGS = -Wall -O2
TARGET = build
SOURCE = ./main.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
