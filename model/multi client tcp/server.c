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
	serv.sin_port = htons(8082);
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv))<0)
	{ printf("[-] Bind Error\n"); return; }
	printf("[+] Bind okay\n");
	
	if (listen(sockfd, 10)<0)
	{ printf("[-] Listen Error\n"); return; }
	printf("[+] Listen okay\n");
	fd_set master, read;
	
	int clients[10], max;
	FD_ZERO(&master);
	FD_SET(sockfd, &master);
	max = sockfd;
	
	while(1)
	{
		read = master;
		if (select(max+1, &read, NULL, NULL, NULL) < 0)
		{ printf("[-] Select error\n"); return;}
		printf("[+] Select ok\n");
		
		for ( int i = 0; i <= max; i++)
		{
			if (FD_ISSET(i, &read))
			{
				if (i == sockfd)
				{
					int len = sizeof(cli);
					int newsockfd = accept(sockfd, (struct sockaddr *)&cli, &len);
					if (newsockfd == -1)
					{ printf("[-] Accept error\n"); return; }
					printf("[+] Accept ok\n");
					FD_SET(newsockfd, &master);
					if (newsockfd > max)
						max = newsockfd;
					printf("New connection from Client %d\n", (cli.sin_addr));
				}
				else
				{
					char msg[30], buf[30];
					bzero(msg, 30);
					recv(i, buf, 30, 0);
					printf("Message from client %d: %s", i, buf);
					int j = 0;
					printf("Message to client %d: ", i);
					while ((msg[j++] = getchar()) != '\n');
					send(i, msg, 30, 0);
					
				}
			}
		}
	}
}