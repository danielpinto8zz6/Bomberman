CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread
SOURCES=login.c  main.c  users.c activeusers.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=../../build/bombermanServer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
