all: quickA quickB

quickA: quickA.o
	gcc209 quickA.o -o quickA

quickB: quickB.o
	gcc209 quickB.o -o quickB

quickA.o: quickA.c
	gcc209 -c -o quickA.o quickA.c

quickB.o: quickB.c
	gcc209 -c -o quickB.o quickB.c

clean:
	rm *.o quickA quickB