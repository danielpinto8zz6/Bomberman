CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread -lncurses
SOURCES=main.c board.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=../../build/bombermanClient

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
