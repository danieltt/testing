all: client server

client: client.c
	$(CC) client.c -o client --static

server: server.c
	$(CC) server.c -o server --static

clean:
	rm -f client server
