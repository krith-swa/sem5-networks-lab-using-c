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

int no_of_domains=0;
table t[15];

int find(char domain[30]){

    for(int i=0;i<no_of_domains;i++){
        if(strcmp(t[i].domain,domain)==0)
            return i;
    }
    return -1;
}

int validate(char ip[15]){
    if(strlen(ip)>15)
        return 0;
    int no_of_dots=0;
    int value=0;
    for(int i=0;i<strlen(ip);i++){
        if(ip[i]!='.'){
            int t=ip[i]-'0';
            value=value*10+t;
        }
        else{
            no_of_dots++;
            if(value<0||value>255)
                return 0;
            value=0;
        }
    }
    if(no_of_dots!=3) {
        return 0;
    }        
    return 1;
}

int main(int argc,char ** argv){

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
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(socket_fd,(struct sockaddr *)&saddr,sizeof(saddr))<0){
        printf("Couldnt bind");
        return 0;
    }
    for(int i=0;i<15;i++){
        t[i].no_of_ip=0;
    }
    strcpy(t[no_of_domains].domain,"www.google.com");
    strcpy(t[no_of_domains].ip[0],"123.6.7.8");
    t[no_of_domains].no_of_ip++;
    no_of_domains++;
    strcpy(t[no_of_domains].domain,"www.amazon.com");
    strcpy(t[no_of_domains].ip[0],"233.60.7.08");
    t[no_of_domains].no_of_ip++;
    no_of_domains++;
    for(int i=0;i<no_of_domains;i++){
        printf("\n------------------------------------------");
        printf("\nDOMAIN NAME: %s",t[i].domain);
        for(int j=0;j<t[i].no_of_ip;j++){
            printf("\nIP ADDRESS %d: %s",j,t[i].ip[j]);
        }
    }
    printf("\n------------------------------------------");
    int choice;
    char buffer[30];
    char domain[30];
    char ip[30];
    int len=sizeof(caddr);
    for(;;){
        len=sizeof(caddr);
        recvfrom(socket_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&caddr,&len);
        printf("\n\nRequired domain: %s",buffer);
        int location=find(buffer);
        if(location>=0){
            printf("\nIP address sent successfully!\n");
            table req=t[location];
            sendto(socket_fd,&req,sizeof(req),0,(struct sockaddr *)&caddr,len);
        }
        else{
            printf("\nNOT FOUND\n");
            table req;
            strcpy(req.domain,"ERROR");
            sendto(socket_fd,&req,sizeof(req),0,(struct sockaddr *)&caddr,len);
        }
        printf("\n1. add\n2. display\nEnter choice: ");
        scanf("%d",&choice);
        if(choice==1){
            printf("\nEnter domain: ");
            scanf("%s",domain);
            int location=find(domain);
            if(location==-1){
                location=no_of_domains;
                no_of_domains++;
                strcpy(t[location].domain,domain);
            }
            printf("Enter ip: ");
            scanf("%s",ip);
            while(!validate(ip)){
                printf("Enter valid ip: ");
                scanf("%s",ip);
            }
            strcpy(t[location].ip[t[location].no_of_ip],ip);
            t[location].no_of_ip++;
        }
        else{
            printf("\n------------------------------------------");
            for(int i=0;i<no_of_domains;i++){
                printf("\nDOMAIN NAME: ");
                puts(t[i].domain);
                for(int j=0;j<t[i].no_of_ip;j++){
                    printf("IP ADDRESS %d: ",j);
                    puts(t[i].ip[j]);
                }
                printf("------------------------------------------");
            }
        }    
    }while(1);
    close(socket_fd);
    return 0;
    
}