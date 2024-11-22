#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "arp.h"

int main(int argc, char **argv){
    
	arp packet, rec;
	packet = createARPPacket(REQ);
	printf("\nDeveloping ARP Request packet\n");
	printPacket(packet);
	printf("\tThe ARP Request packet is broacasted.\n");
	printf("Waiting for ARP Reply...\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		perror("Unable to open socket.\n");
        exit(EXIT_FAILURE);
	}

    struct sockaddr_in server, cli;
	char buffer[1024];
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8086);

	if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0){
		perror("Bind error occurred.\n");
        exit(EXIT_FAILURE);
	}

	listen(sockfd, 10);

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
					{ 
                        printf("[-] Accept error\n"); 
                        return; 
                    }
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
						printPacket(rec);
						strcpy(packet.dest_mac, rec.src_mac);
						packet.mode = DATA;
						printf("Sending Packet back to %s\n", packet.dest_ip);
						send(i, (void *)&packet, sizeof(packet), 0);
						printPacket(packet);
						return;
					} 
				}
			}
		}
	}
    close(sockfd);
	return 0;
}