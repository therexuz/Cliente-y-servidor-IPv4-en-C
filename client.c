#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // write, read, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <string.h> // strlen, memset
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_LENGTH 250
#define FALSE 0
#define SERVER_NAME "ServerHostName"


int main(int argc, char const *argv[]) {
  
  int sd=-1,rc,bytesReceived=0;
  char buffer[BUFFER_LENGTH];
  char server[];
  char servport[] = "3005";
  struct in6_addr serveraddr;
  struct addrinfo hints, *res=NULL;
  
  do{
    
    if(argc > 1)
      strcpy(server, argv[1]);
    else
      strcpy(server,SERVER_NAME);

    memset(&hints,0x00,sizeof(hints));
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    rc = inet_pton(AF_INET6,server,&serveraddr);
    if(rc == 1){
      hints.ai_family = AF_INET6;
      hints.ai_flags |= AI_NUMERICHOST;
    }
    
    rc = getaddrinfo(server,servport,&hints,&res);
    if(rc != 0){
      printf("Host not found : %s\n",gai_strerror(rc));
      if(rc == EAI_SYSTEM){
        perror("getaddrinfo() failed");
      }
      break;
    }

    sd = socket(res->ai_family, res-> ai_socktype, res->ai_protocol);
    if(sd < 0){
      perror("socket () failed");
      break;
    }

    rc = connect(sd,res->ai_addr,res->ai_addrlen);
    if(rc < 0){
      perror("connect failed");
      break;
    }

    memset(buffer,'a',sizeof(buffer));
    rc = send(sd, buffer, sizeof(buffer),0);
    if(rc < 0){
      perror("send() failed");
      break;
    }

    while (bytesReceived < BUFFER_LENGTH){
      rc = recv(sd, & buffer[bytesReceived],BUFFER_LENGTH - bytesReceived,0);
      if(rc < 0){
        perror("recv() failed");
        break;
      }else if(rc == 0){
        printf("The serves closed connection\n");
      }
      bytesReceived += rc;
    }
  }while(FALSE);

  if (sd !=-1){
    close(sd);
  }
  
  if(res != NULL){
    freeaddrinfo(res);
  }
  return 0;
}
