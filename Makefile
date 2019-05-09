CC = gcc
CFLAGS = -Wall -std=c99

all: mta

mta: mta_system.c
	$(CC) $(CFLAGS) -o mta mta_system.c

clean:
	rm *.o mta









