#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <string.h> // memset

int main(int argc, char const *argv[]) {
	
	int serverFd,clientFd;
	struct sockaddr_in6 server, client;
	socklen_t len;
	int port = 1234;
	char buffer[1024];
	
	if (argc == 2){
		port = atoi(argv[1]);
	}

	serverFd = socket(AF_INET6,SOCK_STREAM,0);
	
	if(serverFd < 0){
		perror("!!!!! Cannot create socket !!!!!");
		exit(1);
	}
	
	server.sin6_family = AF_INET6;
	server.sin6_addr = in6addr_any;
	server.sin6_port = htons(port);
	
	if(bind(serverFd,(struct sockaddr *)&server,sizeof(server)) < 0){
		perror("!!!!! Cannot bind socket !!!!!");
		exit(2);
	}

	if(listen(serverFd,10) < 0){
		perror("!!!!! Listen error !!!!!");
		exit(3);
	}

	while(1){
		
		len = sizeof(client);
		printf("Waiting for clients...\n");
		
		if((clientFd=accept(serverFd,(struct sockaddr *)&client,&len)) < 0){
			perror("!!!!! Read error !!!!!");
			exit(4);
		}
		
		char client_ip[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6,&client,client_ip,INET6_ADDRSTRLEN);
		printf("Accepted new connection from a client %s:%d\n",client_ip,ntohs(client.sin6_port));
		int size = read(clientFd,buffer,sizeof(buffer));
		if(size < 0){
			perror("!!!!! Read error !!!!!");
			exit(5);
		}

		printf("received %s from client\n",buffer);
		if(write(clientFd,buffer,size) < 0){
			perror("!!!!! Write error !!!!!");
			exit(6);
		}

		close(clientFd);
		exit(6);
	}
	
	close(serverFd);
	return 0;
}
