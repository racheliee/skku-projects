TARGET=w14
CC=gcc
CFLAGS=-Wall
OBJECTS=w14.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

w14.o: w14.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS) $(TARGET)