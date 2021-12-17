all: bf fw

bf: bf.o
	gcc209 bf.o -g -o bf

bf.o: bf.c
	gcc209 -c -g -o bf.o bf.c

fw: fw.o
	gcc209 fw.o -g -o fw

fw.o: fw.c
	gcc209 -c -g -o fw.o fw.c


clean:
	rm *.o bf fw