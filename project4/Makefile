all: bf_ver3 fw_ver2

bf_ver3: bf_ver3.o
	gcc209 bf_ver3.o -g -o bf_ver3

bf_ver3.o: bf_ver3.c
	gcc209 -c -g -o bf_ver3.o bf_ver3.c

fw_ver2: fw_ver2.o
	gcc209 fw_ver2.o -g -o fw_ver2

fw_ver2.o: fw_ver2.c
	gcc209 -c -g -o fw_ver2.o fw_ver2.c


clean:
	rm *.o bf_ver3 fw_ver2