compiler=gcc
# -DOSX is for Darwin (OS X) only, remove it otherwise
options=-DOSX -Wall -I. -Dstatistics

all: test/test example

test/test: chash.o test/test.c test/test.h
	$(compiler) $(options) test/test.c chash.o -o test/test

example: example.c chash.o
	$(compiler) $(options) chash.o example.c -o example

chash.o: chash.c chash.h
	$(compiler) $(options) -c chash.c

clean:
	- rm chash.o test/test
