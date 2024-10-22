#Makefile
TARGET=w3
CC=gcc
CFLAGS=-Wall
OBJECTS=main.o plus.o minus.o
$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^
main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@
plus.o: plus.c
	$(CC) $(CFLAGS) -c $< -o $@
minus.o: minus.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm $(OBJECTS) $(TARGET)

