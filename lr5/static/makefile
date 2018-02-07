all:
	gcc -c dequeue.c -o static.o
	ar rc libMY_STATIC.a static.o
	ranlib libMY_STATIC.a
	gcc main.c libMY_STATIC.a