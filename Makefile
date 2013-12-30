compiler=gcc
options=-Wall -DOSX
all:
	$(compiler) $(options) chash.c
