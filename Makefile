CC=gcc
OPTIMIZATION=02
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c11 $(OPTIMIZATION)

all: codeStyleChecker

codeStyleChecker : codeStyleChecker.c
	$(CC) $(CFLAGS) codeStyleChecker.c library.c -o codeStyleChecker

clean:
	rm codeStyleChecker