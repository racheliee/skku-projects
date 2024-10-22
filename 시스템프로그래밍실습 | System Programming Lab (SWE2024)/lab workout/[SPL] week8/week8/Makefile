TARGET=w8
GCC=gcc
# CFLAGS=-Wall
LDFLAGS=-lreadline -lhistory
OBJECTS=w8.o

$(TARGET): $(OBJECTS)
	$(GCC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
w8.o: w8.c
	$(GCC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS) $(TARGET)
