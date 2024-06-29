TARGET=w7
CC=gcc
CFLAGS=-Wall
OBJECTS=w7.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
w7.o: w7.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)