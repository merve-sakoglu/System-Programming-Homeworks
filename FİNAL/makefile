all:client.o server.o
	gcc  Client/client.o -o Client/client
	gcc  Server/server.o -o Server/server
client.o:
	gcc -c Client/client.c -o Client/client.o
server.o:
	gcc -c Server/server.c -o Server/server.o
clean:
	rm -r Client/client.o
	rm -r Server/server.o
	rm -r Client/client
	rm -r Server/server
