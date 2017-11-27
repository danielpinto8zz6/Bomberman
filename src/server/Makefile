CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread
SOURCES=database.c  login.c  main.c  register.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=../../build/bombermanServer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
