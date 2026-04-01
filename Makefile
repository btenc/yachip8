CC = gcc
CFLAGS_COMMON = -lSDL2 -Wall -Wextra -pedantic -std=c99
CFLAGS_DEBUG = -g -fsanitize=address -DDEBUG
CFLAGS_RELEASE = -O2

SRC = src/main.c src/chip8.c src/window.c

release:
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_RELEASE) $(SRC) -o yachip8

debug:
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_DEBUG) $(SRC) -o yachip8

clean:
	rm -f yachip8
