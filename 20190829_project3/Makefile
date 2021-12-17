CC = gcc209
VPATH = project3
CPPFLAGS = -I project3
CFLAGS = -D_DEFAULT_SOURCE

all: count

count: count.o
	$(LINK.o) $^ -g -o count

count.o: count.c
	$(CC) -c -g -o count.o count.c

.PHONY: clean
clean:
	rm -f *.o *~