#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

typedef struct table
{
   char dname[30];
   char ipaddr[5][15];
   int np;
   
}table;

int pre=0;
table t1[15];

int find(char domain[30])
{

  for(int i=0;i<=pre;i++)
  {
    if(strncmp(domain,t1[i].dname,strlen(domain))==0)
      return i;
  }
  return -1;
}

int validate(char ip[30])
{
    if(strlen(ip)>15)
      return 0;
      
    int dots=0;
    int k=0,t;
    for(int i=0;i<strlen(ip);i++)
    {	
       if(ip[i]!='.')
     	{ 
        t=ip[i]-'0';
     	  k=k*10+t;
      }
     
      if(ip[i]=='.')
     	{
        dots++; 
       	if((k<0)||(k>255)) 
          return 0;
      	k=0;
     	}
    }
    if(dots!=3)
      return 0;
    return 1;
}

int main()
{
   int sockfds,sockfdc;
   struct sockaddr_in s,c;
   sockfds=socket(AF_INET,SOCK_DGRAM,0);
   bzero(&s,sizeof(s));
   s.sin_family=AF_INET;
   s.sin_port=htons(9001);
   s.sin_addr.s_addr=INADDR_ANY;
   
   printf("SERVER SIDE:\n");
   bind(sockfds,(struct sockaddr*)&s,sizeof(s));
   
   
   for (int i=0;i<15;i++)
   {
     t1[i].np=0;
   }
   
  
   strcpy(t1[pre].dname,"www.yahoo.com");
   strcpy(t1[pre].ipaddr[0],"10.2.45.67");
   t1[pre].np=1;
   
   pre++;
   strcpy(t1[pre].dname,"www.annauniv.edu");
   strcpy(t1[pre].ipaddr[0],"197.34.53.122");
   t1[pre].np=1;
   
   pre++;
   listen(sockfds,5);
   int csize;
   
   int k;
   char ip[15];
   char buffer[30];
   char domain[30];
   for(;;)
   {
      printf("Enter choice:");
      scanf("%d",&k);
      if(k==1)
        {
        printf("Enter domain to be added:");
        scanf("%s",domain);
        if(find(domain)==-1)
        {
          strcpy(t1[pre].dname,domain);
          int val=0;
          while(val!=1){
          printf("Enter ip:");
          scanf("%s",ip);
          val=validate(ip);
          }
          strcpy(t1[pre].ipaddr[t1[pre].np],ip);
          t1[pre].np++;
          pre++;
          
        }
        
        else{
            int loc=find(domain);
            int val=0;
          while(val!=1){
          printf("Enter ip:");
          scanf("%s",ip);
          val=validate(ip);
          }
            strcpy(t1[loc].ipaddr[t1[loc].np],ip);
          t1[loc].np++;
        }
       
        for(int i=0;i<pre;i++)
        {
          printf("DOMAIN NAME:%s\n",t1[i].dname);
          for(int j=0;j<t1[i].np;j++)
            printf("IP%d:%s\n",j+1,t1[i].ipaddr[j]);
          printf("\n\n");
        }
         continue;
        }
      
      csize=sizeof(c);
      bzero(buffer,30);
      recvfrom(sockfds,buffer,sizeof(buffer),0,(struct sockaddr*)&c,&csize);
      //printf("%ld",strlen(buffer));
      buffer[strlen(buffer)-1]='\0';
      printf("Domain from client:%s",buffer);
      if(find(buffer)!=-1)
        {printf("\nIP sent\n");
         table t2=t1[find(buffer)];
        sendto(sockfds,&t2,sizeof(t2),0,(struct sockaddr*)&c,sizeof(c));
        }
      else
      {
      printf("%dErro!!!\n",find(buffer));
       table t2;
       strcpy(t2.dname,"error");
       sendto(sockfds,&t2,sizeof(t2),0,(struct sockaddr*)&c,sizeof(c));
      }

     
   }
   close(sockfds);
   return 0;
   
   
}

