CC = gcc
CFLAGS = -D_GNU_SOURCE -Wall -std=c99 -g
LDLIBS = -lm
attractions: attractions.o point.o pointlist.o
attractions.o: attractions.c point.c point.h pointlist.c pointlist.h
pointlist.o: pointlist.c pointlist.h point.h point.c
point.o: point.c point.h


clean: 
	rm -rf *.o
	rm -rf output.txt
	rm -rf *.o~
	rm -rf *.h~
	rm -rf attractions
