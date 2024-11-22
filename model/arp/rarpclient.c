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
	//packet = genpacket(REQ);
	//print(packet);
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{ printf("[-] Socket error\n"); return; }
	printf("[+] Socket okay\n");
	
	printf("\nEnter the IP Address\t: ");
	scanf("%s", packet.src_ip);
	printf("Enter the MAC Address\t: ");
	scanf("%s", packet.src_mac);
	
	
	struct sockaddr_in serv, cli;
	
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(8086);
	
	connect(sockfd, (struct sockaddr*)&serv, sizeof(serv));
	printf("[+] Connection ok\n");
	
	recv(sockfd, (void*)&rec, sizeof(rec), 0);
	printf("RARP Request Received\n");
	print(rec);
	
	if (strcmp(packet.src_mac, rec.dest_mac) == 0)
	{
		printf("IP Address matches\n");
		packet.mode = ACK;
		strcpy(packet.dest_ip, rec.src_ip);
		strcpy(packet.dest_mac, rec.src_mac);
		send(sockfd, (void *)&packet, sizeof(packet), 0);
		printf("RARP Reply sent\n");
		print(packet);
		bzero(&rec, sizeof(rec));
		recv(sockfd, (void *)&rec, sizeof(rec), 0);
		printf("Received packet with data: \n");
		print(rec);
	}
	
	else
		printf("MAC doesn't match\n");
	close(sockfd);
	return;
}