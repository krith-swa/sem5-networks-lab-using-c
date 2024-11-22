#include<stdio.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int count_bits(long num){
    int count=0;
    while(num>0){
        num=num/10;
        count++;
    }
    return count;
}
int findr(int m){
    int r=1;
    while((int)pow(2,r)<r+m+1){
        r++;
    }
    return r;
}
int r_position(int i){
    return ceil(log2(i))==floor(log2(i));
}
int to_binary(int num){
    int n=0,rem,i=0;
    while(num>0){
        rem=num%2;
        n+=rem*pow(10,i);
        i++;
        num=num/2;
    }
    return n;
}
int ispresent(int num,int pos){
    int rem;
    for(int i=0;i<pos;i++){
        rem=num%10;
        num=num/10;
    }
    if(rem==1){
        return 1;
    }
    else
        return 0;
}
int main(int argc,char **argv){

    int socket_fd;
    struct sockaddr_in saddr,caddr;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1){
        printf("Cannot establish socket");
        return;
    }
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(8080);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(socket_fd,(struct sockaddr *)&saddr,sizeof(saddr))<0){
        printf("Cannot bind");
        return;
    }
    listen(socket_fd,5);

    long data;
    printf("\nEnter data: ");
    scanf("%lu",&data);
    int m=count_bits(data);
    int r= findr(m);
    int data_len=m+r;
    printf("data len= %d\n",data_len);
    int code[data_len];
    int digit;
    for(int i=1;i<=data_len;i++){
        digit=data%10;
        if(r_position(i)){
            code[data_len-i]=2;
        }
        else{
            code[data_len-i]=digit;
            data=data/10;
        }
    }
    printf("\nData with redundant bits: ");
    for(int i = 0; i < data_len; i++)
        printf("%d", code[i]);
    printf("\n");
    int binary,count=0;
    for(int i=0;i<r;i++){
        for(int j=1;j<=data_len;j++){

            if((int)pow(2,i)!=j){
                binary=to_binary(j);
                //printf("\n%d",binary);
                if(ispresent(binary,i+1)){
                    count+=code[data_len-j];
                }
            }
        }

        if(count%2==0){
            code[data_len-(int)pow(2,i)]=0;
        }
        else{
            code[data_len-(int)pow(2,i)]=1;
        }
        count=0;
    }
    printf("\nData with redundant bits: ");
    for(int i = 0; i < data_len; i++)
        printf("%d", code[i]);
    printf("\n");
    long num=0;
    int k=0;
    for(int i=data_len-1;i>=0;i--){
        num+=code[i]*pow(10,k);
        k++;
    }
    char data_s[data_len];
    sprintf(data_s,"%lu",num);
    printf("Data transmitted: %s",data_s);
    
    int len=sizeof(caddr);
    int newfd=accept(socket_fd,(struct sockaddr *)&caddr,&len);
    write(newfd,data_s,sizeof(data_s));
    close(socket_fd);
}