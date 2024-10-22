TARGET=w15
CC=gcc
CFLAGS=-Wall
OBJECTS=w15.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

w15.o: w15.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)