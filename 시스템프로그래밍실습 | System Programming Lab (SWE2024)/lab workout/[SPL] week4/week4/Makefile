TARGET=ex4
CC=gcc
CFLAGS=-Wall
OBJECTS=ex4.o
$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^
ex4.o: ex4.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm $(OBJECTS) $(TARGET)