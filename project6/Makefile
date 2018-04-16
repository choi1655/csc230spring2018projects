CC = gcc
CFLAGS = -Wall -std=c99 -g
regular: regular.o pattern.o parse.o
regular.o: regular.c pattern.c parse.c pattern.h parse.h
pattern.o: pattern.c pattern.h
parse.o: parse.c parse.h

clean:
	rm -rf output.txt
	rm -rf *.o
	rm -rf stderr.txt
	rm -rf stdout.txt
	rm -rf regular
