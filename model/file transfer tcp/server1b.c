#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1024
#define PORT 8080

int main(int argc, char ** argv)
{
	int socket_fd, new_fd, len;
	struct sockaddr_in server_address, client_address;
	char buffer[MAX];
	//establish socket
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		fprintf(stderr, "FAILED TO CREATE SOCKET!\n");
		exit(EXIT_FAILURE);
	}
	//clear address pointer
	bzero(&server_address, sizeof(server_address));
	//assign data members of struct server_address
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT);
	// Binding newly created socket to given IP 
	if ((bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address))) != 0)
	{
		fprintf(stderr,"FAILED TO BIND SOCKET!\n");
		exit(EXIT_FAILURE);
	}
	// limit the number of connects the socket can listed to
	if ((listen(socket_fd, 5)) != 0)
	{
		fprintf(stderr,"FAILED TO LISTEN!\n");
		exit(EXIT_FAILURE);
	}
	len = sizeof(client_address);
	//to accept any connection requests
	new_fd = accept(socket_fd, (struct sockaddr *)&client_address, &len);
	if (new_fd < 0)
	{
		fprintf(stderr,"FAILED TO ACCPET SERVER!\n");
		exit(EXIT_FAILURE);
	}
	//clear buufer
	bzero(buffer, MAX);
	// read the message from client and copy it in buffer
	read(new_fd, buffer, MAX);
	printf("File to be transferred: %s\n", buffer);
	int fd = open(buffer, O_RDONLY);
	bzero(buffer, MAX);
	if (fd < 0){
		strcpy(buffer, "FILE NOT FOUND!");
		fprintf(stderr, "%s\n", buffer);
		write(new_fd, buffer, MAX);
	}
	else{
		read(fd, buffer, MAX);
		printf("File Transferred!!!\n");
		write(new_fd, buffer, MAX);
	}
	close(new_fd);
	close(socket_fd);
}