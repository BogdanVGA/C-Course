
CC = clang

output: idaruser.o intdarray.o
	$(CC) idaruser.o intdarray.o -o output.out

idaruser.o: idaruser.c
	$(CC) idaruser.c -c

intdarray.o: intdarray.c intdarray.h
	$(CC) intdarray.c -c

clean:
	rm *.o *.out