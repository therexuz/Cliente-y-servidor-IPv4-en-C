#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // write, read, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <string.h> // strlen, memset
#include <net/if.h>

const char message[] = "Hello sockets world\n";

int main(int argc, char const *argv[]){
	
	int serverFd;
	struct sockaddr_in6 server;
	int port = 1234;
	char const * server_ip = "fe80::a00:27ff:fedb:3d46";
	char *buffer = "Hello server";
	
	if(argc == 3){
		server_ip = argv[1];
		port = atoi(argv[2]);
	}

	serverFd = socket(AF_INET6,SOCK_STREAM,0);
	
	if(serverFd < 0){
		perror("Connect create socket");
		exit(1);
	}
	
	server.sin6_family = AF_INET6;
	inet_pton(AF_INET6,server_ip,&server.sin6_addr);
	server.sin6_port = htons(port);
	server.sin6_scope_id = if_nametoindex("enp0s3");

	if(connect(serverFd,(struct sockaddr *)&server,sizeof(server)) < 0){
		perror("Cannot connect to server");
		exit(2);
	}

	if(write(serverFd,buffer,strlen(buffer)) < 0){
		perror("Cannot write");
		exit(3);
	}
	
	char recv[1024];
	
	if(read(serverFd,recv,sizeof(recv)) < 0){
		perror("Cannot read");
		exit(4);
	}
	
	printf("Received %s from server\n", recv);
	
	close(serverFd);

	return 0;
}
