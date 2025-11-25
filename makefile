CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = ngr

all: $(TARGET)

$(TARGET): nuguri.c
	$(CC) $(CFLAGS) -o $(TARGET) nuguri.c

clean:
	rm -f $(TARGET)
