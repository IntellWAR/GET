all:
	gcc ssh_client.c -c -o ssh_client.o
	gcc ssh_server.c -c -o ssh_server.o
	gcc ssh_client.o -o ssh_client
	gcc ssh_server.o -o ssh_server -lpam -lpam_misc
