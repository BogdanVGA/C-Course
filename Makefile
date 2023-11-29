
CC = clang

output: dyn_array_main.o dyn_array.o
	$(CC) dyn_array_main.o dyn_array.o -o dyn_array.out

dyn_array_main.o: dyn_array_main.c
	$(CC) dyn_array_main.c -c

dyn_array.o: dyn_array.c dyn_array.h
	$(CC) dyn_array.c -c

clean:
	rm *.o *.out