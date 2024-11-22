#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

typedef struct table{
    char domain[30];
    char ip[5][15];
    int no_of_ip;
}table;

int main(int argc,char **argv){

    int socket_fd;
    struct sockaddr_in saddr,caddr;
    socket_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(socket_fd==-1){
        printf("Cannot establish sokcet!\n");
        return 0;
    }
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(8080);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int choice;
    char buffer[50];
    char domain[30];
    char ip[20];
    int len=sizeof(saddr);
    while(1){
        bzero(&buffer,50);
        printf("\nEnter domain: ");
        scanf("%s",buffer);
        sendto(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&saddr,len);
        table t;
        recvfrom(socket_fd,&t,sizeof(t),0,(struct sockaddr *)&saddr,&len);
        if(strcmp(t.domain,"ERROR")==0){
            printf("\nDomain doesnt exist!");
        }
        else{
            printf("\n------------------------------------------");
            {
                printf("\nDOMAIN NAME: %s",t.domain);
                for(int j=0;j<t.no_of_ip;j++)
                    printf("\nIP ADDRESS %d: %s",j+1,t.ip[j]);
                printf("\n------------------------------------------\n");
            }
        }

    }
    close(socket_fd);
    return 0;
    
}