#include <stdio.h> //perror,printf0
#include <stdlib.h> //exit, atoi
#include <unistd.h> //read, write, close
#include <arpa/inet.h> //sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY, socket, etc...
#include <string.h> //memset

int main(int argc,char const *argv[]){
	
	int serverFd;
	struct sockaddr_in6 server, client;
	socklen_t len;
	int port = 1234;
	char buffer[1024];
	char * hello = "Hello client :D";

	if(argc == 2){
		port = atoi(argv[1]);
	}
	
	serverFd = socket(AF_INET6,SOCK_DGRAM,0);
	
	if(serverFd<0){
		perror("Cannot create socket");
		exit(1);
	}
	
	server.sin6_family = AF_INET6;
	server.sin6_addr = in6addr_any;
	server.sin6_port = htons(port);

	len = sizeof(server);

	if(bind(serverFd,(struct sockaddr *)&server,len)<0){
	perror("Cannot bin socket");
		exit(2);
	}

	while(1){
		printf("Waiting for clients...\n");
		len = sizeof(client);
		memset(&client,0,sizeof(client));
		int n;
		n = recvfrom(serverFd,(char *)buffer,1024,0,(struct sockaddr *)&client,&len);
		char client_ip[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6,&client,client_ip,INET6_ADDRSTRLEN); 
		printf("Accepted new connection from a client %s%:d\n",client_ip,ntohs(client.sin6_port));
		buffer[n] ='\0';
		printf("Received message from Client: %s\n",buffer);
		sendto(serverFd,(const char *)hello,strlen(hello),MSG_CONFIRM,(const struct sockaddr *)&client,len);
	}
	
	close(serverFd);
	return 0;
}
