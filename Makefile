CC=gcc
OBJECTS=list.o main.o

all: test

test: $(OBJECTS)
	$(CC) -o $@ -std=c89 -Wall -pedantic $^

%.o:	%.c
	$(CC) -c -std=c89 -Wall -pedantic $<

clean:
	@rm -f test list *.o
	@echo "All squeaky clean!"
