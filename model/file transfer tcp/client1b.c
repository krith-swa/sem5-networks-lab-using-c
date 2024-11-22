#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define  MAX 1024
#define PORT 8080

int main(int argc, char ** argv)
{
	int socket_fd;
	struct sockaddr_in server_address;
	char buffer[MAX], filename[30];
	//establish socket
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		fprintf(stderr,"FAILED TO CREATE SOCKET!\n");
		exit(0);
	}
	//clear address pointer
	bzero(&server_address, sizeof(server_address));
	// assign data member of struct server_address
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(PORT);
	// connect the client socket to server socket
	if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
	{
		fprintf(stderr,"FAILED TO CONNECT!\n");
		exit(0);
	}
	//read path name
	printf("Enter the path of the file to transfer in Server: ");
	scanf("%[^\n]", buffer);
	getchar();
	write(socket_fd, buffer, MAX);
	read(socket_fd, buffer, MAX);
	if(strcmp(buffer, "FILE NOT FOUND!")==0){
		fprintf(stderr, "%s\n", buffer);
		exit(EXIT_FAILURE);
	}
	else{
		printf("File Transferred\nEnter path to store the file in Client: ");
		scanf("%[^\n]", filename);
		int fd = creat(filename,S_IRWXU);
		if (fd < 0){
			fprintf(stderr, "CANNOT CREATE FILE!\n");
			exit(EXIT_FAILURE);
		}
		write(fd, buffer, strlen(buffer));
		close(fd);
	}
	close(socket_fd);
}