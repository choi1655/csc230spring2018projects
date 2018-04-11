CC = gcc
CFLAGS = -Wall -std=c99 -g

all: encode decode
encode: encode.o bits.o codes.o
decode: decode.o bits.o codes.o
encode.o: encode.c codes.c bits.c codes.h bits.h
decode.o: decode.c codes.c bits.c codes.h bits.h
bits.o: bits.c bits.h
codes.o: codes.c codes.h

clean:
	rm -rf encode
	rm -rf decode
	rm -rf output.txt
	rm -rf *.o
	rm -rf stderr.txt
	rm -rf stdout.txt
