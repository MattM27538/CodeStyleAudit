CC=gcc
OPTIMIZATION=02
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c11 $(OPTIMIZATION)

CFILES=codeStyleChecker.o library.o
OBJECTFILES=

BINARY=codeStyleChecker

all : $(BINARY)

$(BINARY) : $(OBJECTFILES)
	$(CC) -o $@ $^

%o : %c
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -rf $(BINARY) $(OBJECTFILES)