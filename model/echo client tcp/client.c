#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAX 50

int main(int argc, char **argv){
    int socket_fd;
    struct sockaddr_in saddr;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1){
        printf("socket not established!");
        exit(0);
    }
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(socket_fd,(struct sockaddr *)&saddr,sizeof(saddr))!=0){
        printf("Cannot connect!");
        exit(0);
    }
    char buff[100];
    printf("\nClient: ");
    scanf("%[^\n]",buff);
    write(socket_fd,buff,sizeof(buff));
    read(socket_fd,buff,sizeof(buff));
    printf("\nServer: %s\n",buff);
    close(socket_fd);
}