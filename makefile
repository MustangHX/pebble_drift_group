all:
#gcc -E main.c -o main.o
#	gcc -E drift.c -o drift.o
#	gcc -c main.c
#	gcc -c drift.c
#	gcc -o main.o drift.o -o pebble
	gcc *.c -o pebble

clean:
	rm -rf *.o pebble
