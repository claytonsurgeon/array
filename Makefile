


all:
	gcc -O0 -Wall ./main.c -g -o ./build/array
	./build/array


gdb:
	gcc -O0 -Wall ./main.c -g -o ./build/array
	gdb ./build/array

