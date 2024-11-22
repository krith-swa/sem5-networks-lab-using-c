#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netdb.h>
#define PORT 8080

int main(int argc,char **argv){
    
    int socket_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(socket_fd<0){
        printf("Socket not established!");
        return 0;
    }
    char buffer[50];
    struct sockaddr_in saddr;
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bzero(buffer,sizeof(buffer));
    while(1){
        
        printf("\nClient 1: ");
        scanf("%[^\n]",buffer);
        getchar();
        sendto(socket_fd,buffer,strlen(buffer),0,(struct sockaddr *)&saddr,sizeof(saddr));
        int len=sizeof(saddr);
        bzero(buffer,sizeof(buffer));
        int n=recvfrom(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&saddr,&len);
        printf("From server: %s",buffer);
        bzero(buffer,sizeof(buffer));
    }  
    printf("\n");
    close(socket_fd);
    return 0;
}