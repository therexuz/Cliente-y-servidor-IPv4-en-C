#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <string.h> // memset
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define SERVER_PORT 3005
#define BUFFER_LENGTH 250
#define FALSE 0

int main(int argc, char const *argv[]) {
  int sd=-1, sdconn=-1;
  int rc, on=1, rcdsize=BUFFER_LENGTH;
  char buffer[BUFFER_LENGTH];
  struct sockaddr_in6 serveraddr, clientaddr;
  int addrlen=sizeof(clientaddr); 
  char str[INET6_ADDRSTRLEN];
  
  do{
    
    if((sd = socket(AF_INET6,SOCK_STREAM,0)) < 0){
      perror("socket() failed");
      break;
    }
    
    if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on)) < 0){
      perror("setsockopt(SO_REUSEADDR) failed");
      break;
    }
    
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_port = htons(SERVER_PORT); 
    
    if(bind(sd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0){
      perror("bind() failed");
      break;
    }
    
    if(listen(sd,10) < 0){
      perror("listen() failed");
      break;
    }

    printf("Waiting for client connection...\n");
    
    if((sdconn = accept(sd,NULL,NULL)) < 0){
      perror("accept() failed");
      break;
    }
    
    getpeername(sdconn,(struct sockaddr *)&clientaddr,&addrlen);
    if(inet_ntop(AF_INET6,&clientaddr.sin6_addr,str,sizeof(str))){
      printf("client address is %s\n",str);
      printf("client port is %d\n",ntohs(clientaddr.sin6_port));
    }
    
    memset(buffer,0,sizeof(buffer));
    int size = read(sdconn,buffer,sizeof(buffer));
    if( size < 0){
      perror("read error");
      break;
    }
    printf("received %s from client\n", buffer);
    if(write(sdconn,buffer,size) < 0){
      perror("write error");
      break;
    }
    close(sdconn);
    
  }while(FALSE);
  close(sd);
  return 0;
}
