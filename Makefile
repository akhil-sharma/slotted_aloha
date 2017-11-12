CFLAGS = -Wall -std=c99

CC = gcc

all: aloha.o file_writer.o
	$(CC) $(CFLAGS) -o aloha aloha.o file_writer.o

file_writer.o: file_writer.c
	$(CC) $(CFLAGS) -c file_writer.c

aloha.o: aloha.c
	$(CC) $(CFLAGS) -c aloha.c

clean:
	rm -f core *.o