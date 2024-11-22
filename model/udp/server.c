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
    
    struct sockaddr_in saddr,caddr;
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    
    if(bind(socket_fd,(struct sockaddr *)&saddr,sizeof(saddr))<0){
        printf("Couldnt bind!");
        return 0;
    }
    
    fd_set rset;
    FD_ZERO(&rset);
    int maxfd=socket_fd+1;
    int len=sizeof(caddr);
    while(1){
        FD_SET(socket_fd,&rset);
        int n=select(maxfd,&rset,NULL,NULL,NULL);        
        if(FD_ISSET(socket_fd,&rset)){
            len = sizeof(caddr);
            bzero(buffer,sizeof(buffer));
            printf("\nFrom Client: ");
            int n=recvfrom(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&caddr,&len);
            puts(buffer);
            printf("\nServer: ");
            puts(buffer);
            //scanf("%[^\n]",buffer);
            sendto(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&caddr,len);
            bzero(buffer,sizeof(buffer));
        }
    }
    close(socket_fd);
    return 0;

}