#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#define MAX 50
#define PORT 8080

int main(int argc, char **argv){

    struct sockaddr_in saddr,caddr;
    int socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1){
        printf("Socket not established!");
        exit(0);
    }

    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(socket_fd,(struct sockaddr *)&saddr,sizeof(saddr))!=0){
        printf("\nFailed to bind");
        exit(0);
    }

    if(listen(socket_fd,5)!=0){
        printf("failed to listen");
        exit(0);
    }
    int len=sizeof(caddr);
    int new_fd=accept(socket_fd,(struct sockaddr *)&caddr,&len);
    if(new_fd<0){
        printf("couldnt connect");
        exit(0);
    }

    char buffer[50];
    bzero(&buffer,50);
    read(new_fd,buffer,sizeof(buffer));
    printf("\nfrom Client: %s\n",buffer);
    //bzero(&buffer,50);
    //printf("Server: ");
    //scanf("%[^\n]",buffer);
    write(new_fd,buffer,sizeof(buffer));
    close(socket_fd);
}