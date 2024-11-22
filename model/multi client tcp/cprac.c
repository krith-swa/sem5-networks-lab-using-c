#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 30
#define PORT 8082

int main() {
    int sock_fd;
    struct sockaddr_in serv, cli;
    char buff[MAX], msg[MAX];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 1) {
        printf("socket error\n");
        exit(0);
    }
    printf("socket\n");

    bzero(&serv, sizeof(serv));

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (connect(sock_fd, (struct sockaddr*) &serv, sizeof(serv)) < 0) {
        printf("connect error\n");
        exit(0);
    }
    printf("connect\n");

    while(1) {
        bzero(buff, MAX);
        bzero(msg, MAX);

        int j = 0;
        printf("Client: ");
        while((msg[j++] = getchar()) != '\n'); 
        send(sock_fd, msg, MAX, 0);

        recv(sock_fd, buff, MAX, 0);
        printf("Message from server: %s\n", buff);
    }

    return 0;
}