all: client server

client:
	gcc -g client.c -o client -lzmq 

server: 
	gcc -g server.c bank.c -o server -lzmq