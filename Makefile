CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=database.c  login.c  main.c  register.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bombermanServer

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm bombermanServer *o
