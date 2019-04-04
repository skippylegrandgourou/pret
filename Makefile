CFLAGS=-Wall -pedantic -std=c11 -lm

pret: main.o
	gcc $(CFLAGS) -o pret main.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	rm -f pret *.o
