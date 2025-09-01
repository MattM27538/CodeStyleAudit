BINARY=codeStyleChecker
CODEDIRECTORIES=.
INCLUDEDIRECTORIES=.

CC=gcc
OPT=-O2

DEPENDENCYFLAGS=-MP -MD 
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c11 -g $(foreach DIRECTORY, $(INCLUDEDIRECTORIES), -I$(DIRECTORY)) $(OPT) $(DEPENDENCYFLAGS) #change D

CFILES=$(foreach DIRECTORY, $(CODEDIRECTORIES), $(wildcard $(DIRECTORY)/*.c)) #change D

OBJECTS=$(patsubst %.c, %.o, $(CFILES))
DEPENDENCYFILES=$(patsubst %.c, %.d, $(CFILES))

all : $(BINARY)

$(BINARY) : $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPENDENCYFILES)