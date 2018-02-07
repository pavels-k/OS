all:
	gcc -fPIC -c dequeue.c -o dynamic_dq.o
	gcc -shared -o libDYN_DQ.so dynamic_dq.o
	gcc main2.c -ldl