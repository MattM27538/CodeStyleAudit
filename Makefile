CC=gcc
INCDIRS=-I.
OPTIMIZATION=-O2
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c11 -g $(INCDIRS) $(OPTIMIZATION)

CFILES=codeStyleChecker.c library.c
OBJECTFILES=codeStyleChecker.o library.o

BINARY=codeStyleChecker

all : $(BINARY)

$(BINARY) : $(OBJECTFILES)
	$(CC) -o $@ $^

%o : %c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECTFILES)