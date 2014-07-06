CC=gcc
OBJECTS=list.o tests.o

all: tests

tests: $(OBJECTS)
	$(CC) -o $@ $^

%.o:	%.c
	$(CC) -c -g -Wall -Wextra -Werror -std=c89 -pedantic-errors $<

clean:
	@rm -f tests list *.o
	@echo "All squeaky clean!"
