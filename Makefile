CC=gcc
CFLAGS=-Wall -Wextra -march=native -mtune=native -O3 -flto -funroll-all-loops

all:
	$(CC) brexit.c -o brexit $(CFLAGS)
clean:
	rm brexit
