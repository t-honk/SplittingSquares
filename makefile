SHELL = /bin/sh
CC = gcc
CFLAGS = -O3 -Wall
SRCDIR = ./src
BUILDDIR = build
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
RM = rm -f

.PHONY: all windows linux clean

all: windows

directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

windows: directories $(BINDIR)/out.exe

$(BINDIR)/out.exe: $(OBJECTS)
	$(CC) -o $@ $^ -lSDL2 -lm

linux: directories $(BINDIR)/out-linux

$(BINDIR)/out-linux: $(OBJECTS)
	$(CC) -o $@ $^ -lSDL2 -lm

clean:
	$(RM) -r $(BUILDDIR) $(BINDIR)