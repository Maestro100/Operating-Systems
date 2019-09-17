gcc -c malloc.c -o malloc.o
ar rcs libmalloc.a malloc.o
gcc -c check.c -o check.o
gcc -o check check.o -L. -lmalloc