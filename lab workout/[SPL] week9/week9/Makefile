TARGET=w9
CC=gcc
# CFLAGS=-Wall
OBJECTS=w9.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
w9.o: w9.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)