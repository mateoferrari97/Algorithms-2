CC = gcc
CFLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion

bf: pila.o brainfuck.o

fixcol: fixcol.o

strutil: strutil.o
