# Simple makefile for chibicc - see Makefile_orig.txt
# chibicc
# % make

CFLAGS=-std=c11 -fno-common -Wall -Wno-switch

all: chibicc

chibicc: chibicc.h main.c codegen.c parse.c unicode.c type.c tokenize.c strings.c preprocess.c hashmap.c
	@echo 'Building chibicc.'
	gcc $(CFLAGS) -o chibicc main.c codegen.c parse.c unicode.c type.c tokenize.c strings.c preprocess.c hashmap.c

clean:
	rm -f chibicc
