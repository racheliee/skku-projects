# Compiler
CC=gcc
CFLAGS=-Wall -std=gnu99
LDFLAGS=-lreadline -lhistory

# Directories
SHELLDIR=shell_src
EXECDIR=executable_src
BINDIR=bin

# Source files
SHELL_OBJS=$(wildcard $(SHELLDIR)/*.c)
EXEC_OBJS=$(wildcard $(EXECDIR)/*.c)

# Executables
EXECUTABLES=$(BINDIR)/pa2 $(EXEC_OBJS:$(EXECDIR)/%.c=$(BINDIR)/%)
EXECUTABLE_NAMES=$(EXEC_SRCS:$(EXECDIR)/%.c=%)

# Targets
all: $(EXECUTABLES)

$(BINDIR)/pa2: $(SHELL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/%: $(EXECDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)/*

pa2: all