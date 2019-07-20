CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion
ARC.C = hash.c abb.c heap.c cola.c pila.c lista.c strutil.c tp2.c
ARC.H = hash.h abb.h heap.h cola.h pila.h lista.h strutil.h 

build: $(ARC.C) $(ARC.H) 
	$(CC) $(CFLAGS) $(ARC.C) -o pruebas
	
run: build
	./pruebas
