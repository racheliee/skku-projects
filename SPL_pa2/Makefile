# Compiler
CC=gcc
CFLAGS=-Wall -std=gnu99 -D_GNU_SOURCE
LDFLAGS=-lreadline -lhistory

# Directories
SHELLDIR=shell_src
EXECDIR=executable_src
BINDIR=bin

# Source files
SHELL_SRC=$(wildcard $(SHELLDIR)/*.c)
EXEC_SRC=$(wildcard $(EXECDIR)/*.c)

# Object files
SHELL_OBJS=$(SHELL_SRC:$(SHELLDIR)/%.c=$(BINDIR)/%.o)
EXEC_OBJS=$(EXEC_SRC:$(EXECDIR)/%.c=$(BINDIR)/%)

# Executables
EXECUTABLES=$(BINDIR)/pa2 $(EXEC_OBJS)

# Targets
all: $(EXECUTABLES)

$(BINDIR)/pa2: $(SHELL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/%: $(BINDIR)/%.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/%.o: $(SHELLDIR)/%.c $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/%.o: $(EXECDIR)/%.c $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)

pa2: all


# Compiler
# CC=gcc
# CFLAGS=-Wall -std=gnu99
# LDFLAGS=-lreadline -lhistory

# # Directories
# SHELLDIR=shell_src
# EXECDIR=executable_src
# BINDIR=bin

# # Source files
# SHELL_OBJS=$(wildcard $(SHELLDIR)/*.c)
# EXEC_OBJS=$(wildcard $(EXECDIR)/*.c)

# # Executables
# EXECUTABLES=$(BINDIR)/pa2 $(EXEC_OBJS:$(EXECDIR)/%.c=$(BINDIR)/%)
# EXECUTABLE_NAMES=$(EXEC_SRCS:$(EXECDIR)/%.c=%)

# # Targets
# all: $(EXECUTABLES)

# $(BINDIR)/pa2: $(SHELL_OBJS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# $(BINDIR)/%: $(EXECDIR)/%.c
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# $(BINDIR):
# 	mkdir -p $(BINDIR)

# clean:
# 	rm -rf $(BINDIR)/*

# pa2: all

