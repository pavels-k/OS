all: client server

client:
	gcc -g client.c -o client -lzmq 

server: 
	gcc -g server.c stats.c -o server -lzmq