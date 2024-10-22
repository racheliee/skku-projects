#Makefile
TARGET=pa1
CC=gcc
CFLAGS=-Wall
OBJECTS=main.o func.o

$(TARGET): $(OBJECTS) func.h
	$(CC) $(CFLAGS) -o $@ $^
main.o: main.c func.h
	$(CC) $(CFLAGS) -c $< -o $@
func.o: func.c func.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)