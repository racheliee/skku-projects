TARGET=cron
CC=gcc
CFLAGS=-Wall
OBJECTS=cron.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
cron.o: cron.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)