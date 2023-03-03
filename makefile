CC=gcc
CCFLAGS=-Wall -Werror -std=c99

all: process_tracker.o getinfo.o
	$(CC) $^ -o all
%.o: %.c getinfo.h
	$(CC) $(CCFLAGS) $< -c

.PHONY: clean
clean:
	rm -f *.o all