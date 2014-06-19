CC=gcc
OBJECTS=list.o main.o

all: test

test: $(OBJECTS)
	$(CC) -o $@ $^

%.o:	%.c
	$(CC) -c -Wall -Wextra -Werror -std=c89 -pedantic-errors $<

clean:
	@rm -f test list *.o
	@echo "All squeaky clean!"
