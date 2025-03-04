CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = scheduler
SRC = scheduler.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
