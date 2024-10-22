TARGET=w12
CC=gcc
CFLAGS=-Wall -Wextra -Werror
OBJECTS=w12.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -pthread

w12.o: w12.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)