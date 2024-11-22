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

    if (bind(sock_fd, (struct sockaddr*) &serv, sizeof(serv)) < 0) {
        printf("bind error\n");
        exit(0);
    }
    printf("bind\n");

    if (listen(sock_fd, 10) < 0) {
        printf("listen error\n");
        exit(0);
    }
    printf("listen\n");

    fd_set master, read;
    int clients[10], max;
    FD_ZERO(&master);
    FD_SET(sock_fd, &master);
    max = sock_fd;

    while(1) {
        read = master;
        if (select(max+1, &read, NULL, NULL, NULL) < 0) {
            printf("select error\n");
            exit(0);
        }
        printf("select\n");

        for (int i=0; i<=max; i++) {
            if (FD_ISSET(i, &read)) {
                if (i == sock_fd) {
                    int len = sizeof(cli);
                    int newsock_fd = accept(sock_fd, (struct sockaddr*) &cli, &len);
                    if (newsock_fd < 0) {
                        printf("accept error\n");
                        exit(0);
                    }
                    printf("accept\n");

                    FD_SET(newsock_fd, &master);
                    if (newsock_fd > max) 
                        max = newsock_fd;
                    printf("New conn from client: %d\n", cli.sin_addr);
                }
                else {
                    bzero(buff, MAX);
                    bzero(msg, MAX);

                    recv(i, buff, MAX, 0);
                    printf("Message from client %d: %s\n", i, buff);

                    int j = 0;
                    printf("Server: ");
                    while((msg[j++] = getchar()) != '\n');
                    send(i, msg, MAX, 0);
                }
            }
        }
    }

    return 0;
}