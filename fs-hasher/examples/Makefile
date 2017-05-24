CC=gcc

all: tool

read_hashfile: read_hashfile.c
	$(CC) -Wall read_hashfile.c -L ../ -lhashfile -o read_hashfile

tool: tool.c
	$(CC) -Wall tool.c -L ../ -lhashfile -o TOOL

clean:
	rm -f read_hashfile
	rm -f TOOL
