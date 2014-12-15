CC=gcc
UNIT-TESTS-OBJECTS=list.o tests.o
INTEGRATION-TEST-OBJECTS=list.o integration-test.o

all: unit-tests integration-test

unit-tests: $(UNIT-TESTS-OBJECTS)
	$(CC) -o $@ $^

integration-test: $(INTEGRATION-TEST-OBJECTS)
	$(CC) -o $@ $^

%.o:	%.c
	$(CC) -c -g -Wall -Wextra -Werror -std=c89 -pedantic-errors $<

clean:
	@rm -f unit-tests integration-test list *.o
	@echo "All squeaky clean!"
