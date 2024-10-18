#    Apodoulianakis Marios   
#    csd4801@csd.uoc.gr            
#    October 2024       
#    CS345 Fall Semester 2024

CC = gcc
CFLAGS = -c -Wall

all: hy345sh clean

hy345sh: hy345sh.o
	$(CC) hy345sh.o -o hy345sh

hy345sh.o: hy345sh.c
	$(CC) $(CFLAGS) hy345sh.c

clean:
	rm -rf *.o

flush:
	rm -rf hy345sh
