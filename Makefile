main: main.o api.o
	gcc -o main main.o api.o
	
main.o: main.c api.h
	gcc -Wall -c main.c
	
api.o: api.c api.h
	gcc -Wall -c api.c

clean:
	rm -f *.o