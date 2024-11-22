#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main(int argc, char **argv){
    
    struct sockaddr_in saddr;
    struct hostent *address;
    struct in_addr ip_addr;
    int socket_fd;
    char name[50],ip[50],domain[50],path[50];

    if(argc!=2){
        printf("name of the server not passed!\n");
        return;
    }

    strcpy(name,argv[1]);
    char * token=strtok(name,"/");
    if(token){
        strcpy(domain,token);
        token=strtok(NULL,"");
    }
    strcpy(path,token);
    
    address=gethostbyname(domain);
    ip_addr=*(struct in_addr *)(address->h_addr);
    strcpy(ip,inet_ntoa(ip_addr));

    socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1){
        printf("socket not established\n");
        return;
    }
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(80);
    saddr.sin_addr.s_addr=inet_addr(ip);
    if(connect(socket_fd,(struct sockaddr *)&saddr,sizeof(saddr))!=0){
        printf("couldnt connect\n");
        return;
    }
    char request[1000],reply[1000],filename[20];
    snprintf(request,99,"GET /%s HTTP/1.1\r\nHost: %s \r\n\r\n",path,domain);

    if(send(socket_fd,request,strlen(request),0)<0){
        printf("Request failed\n");
        return;
    }

    printf("enter filename: ");
    scanf("%s",filename);
    FILE* fp=fopen(filename,"w+");
    int byte_read;
    while(byte_read=read(socket_fd,reply,sizeof(reply))>0){
        fputs(reply,fp);
        memset(reply,0,sizeof(reply));
    }
    printf("Downloaded");
    fclose(fp);
    close(socket_fd);

}