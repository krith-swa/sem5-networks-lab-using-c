#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

void main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{ printf("[-] Socket error\n"); return; }
	printf("[+] Socket okay\n");
	
	struct sockaddr_in serv, cli;
	
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8082);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	connect(sockfd, (struct sockaddr*)&serv, sizeof(serv));
	//if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) == -1 );
	//{ printf("[-] Connection failed\n"); return;}
	printf("[+] Connection ok\n");
	
	while (1)
	{
		char msg[30], buf[30];
		bzero(msg, 30);
		int j = 0;
		printf("Message to server: ");
		while ((msg[j++] = getchar())!='\n');
		send(sockfd, msg, 30, 0);
		recv(sockfd, buf, 30, 0);
		printf("Message from server: %s\n", buf);
		if (strcmp(buf, "bye") == 0)
		{
			printf("Exiting!\n");
			return;
		}
	} 
}