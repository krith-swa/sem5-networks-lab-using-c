#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include "rarp.h"

void main(int argc, char **argv)
{
	rarp packet, rec;
	packet = genpacket(REQ);
	print(packet);
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{ printf("[-] Socket error\n"); return; }
	printf("[+] Socket okay\n");
	
	struct sockaddr_in serv, cli;
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8086);
	serv.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) == -1)
	{ printf("[-] Bind Error\n"); return; }
	printf("[+] Bind okay\n");
	
	if (listen(sockfd, 10) == -1)
	{ printf("[-] Listen Error\n"); return; }
	printf("[+] Listen okay\n");
	
	fd_set master, read;
	FD_ZERO(&master);
	FD_SET(sockfd, &master);
	int max = sockfd;
	
	while(1)
	{
		read = master;
		if (select(max+1, &read, NULL, NULL, NULL) == -1)
		{ printf("[-] Select error\n"); return;}
		//printf("[+] Select ok\n");
		
		for (int i = 0 ; i<=max; i++)
		{
			if (FD_ISSET(i, &read))
			{
				if ( i == sockfd )
				{
					int len = sizeof(cli);
					int newfd = accept(sockfd, (struct sockaddr*)&cli, &len);
					if (newfd == -1)
					{ printf("[-] Accept error\n"); return; }
					printf("[+] Accept with %d ok\n", inet_ntoa(cli.sin_addr));
					FD_SET(newfd, &master);
					if (newfd > max)
						max = newfd;
					send(newfd, (void*)&packet, sizeof(packet), 0);
				}
				else
				{
					recv(i, (void *)&rec, sizeof(rec), 0);
					if (rec.mode == ACK)
					{
						printf("Acknowledgement received\n");
						print(rec);
						strcpy(packet.dest_ip, rec.src_ip);
						packet.mode = DATA;
						printf("Sending Packet back to %s\n", packet.dest_ip);
						send(i, (void *)&packet, sizeof(packet), 0);
						print(packet);
						return;
					} 
				}
			}
		}
	}
	close(sockfd);
	return;
}