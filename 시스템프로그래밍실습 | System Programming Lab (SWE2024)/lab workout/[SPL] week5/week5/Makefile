TARGET=w5
CC=gcc
CFLAGS=-Wall
LDFLAGS=-lreadline
OBJECTS=week5.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
week5.o: week5.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)