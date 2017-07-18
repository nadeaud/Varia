
CC=gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)

all: filelinux filemmap server

file_linux.o : file_linux.c
	$(CC) $(CFLAGS) file_linux.c -o file_linux.o

file_mmap.o : file_mmap.c
	$(CC) $(CFLAGS) file_mmap.c -o file_mmap.o

file_server.o : file_server.c
	$(CC) $(CFLAGS) file_server.c -o file_server.o

filelinux: file_linux.o
	$(CC) $(LFLAGS) file_linux.o -o linuxfile

filemmap: file_mmap.o
	$(CC) $(LFLAGS) file_mmap.o -o mmapfile

server: file_server.o
	$(CC) $(LFLAGS) file_server.o -o server

clean:
	\rm *.o linuxfile mmapfile server
