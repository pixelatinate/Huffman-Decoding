CC = gcc 
INCLUDES = -I/home/jplank/cs360/include
CFLAGS = $(INCLUDES) -g -lm -Wall -Wextra -o
LIBDIR = /home/jplank/cs360/lib
LIBS = $(LIBDIR)/libfdr.a 

EXECUTABLES = bin/huff_dec

all: $(EXECUTABLES)

bin/huff_dec: src/huff_dec.c
	$(CC) $(CFLAGS) bin/huff_dec src/huff_dec.c $(LIBS)

clean:
	rm core $(EXECUTABLES) *.o
