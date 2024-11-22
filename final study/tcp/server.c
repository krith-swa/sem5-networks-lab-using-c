#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080

int main(int argc, char* argv[]) {
    int sockfd, newsockfd;
    struct sockaddr_in serv, cli;
    char buf[MAX], msg[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("socket error\n");
        exit(0);
    }
    printf("socket\n");

    bzero(&serv, sizeof(serv));

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*) &serv, sizeof(serv)) < 0) {
        printf("bind error\n");
        exit(0);
    }
    printf("bind\n");

    if (listen(sockfd, 10) < 0) {
        printf("listen error\n");
        exit(0);
    }
    printf("listen\n");

    fd_set master, read;
    int clients[10], max;
    FD_ZERO(&master);
    FD_SET(sockfd, &master);
    max = sockfd;

    while(1) {
        read = master;
        if (select(max + 1, &read, NULL, NULL, NULL) < 0) {
            printf("select error\n");
            exit(0);
        }
        printf("select\n");

        for (int i=0; i<=max; i++) {
            if (FD_ISSET(i, &read)) {
                if (i==sockfd) {
                    int len = sizeof(cli);
                    newsockfd = accept(sockfd, (struct sockaddr*) &cli, &len);
                    if (newsockfd < 0) {
                        printf("accept error\n");
                        exit(0);
                    }
                    printf("accept\n");

                    FD_SET(newsockfd, &master);
                    if (newsockfd > max)
                        max = newsockfd;
                    printf("\nNew client!\n");
                }
                else {
                    bzero(msg, sizeof(msg));
                    bzero(buf, sizeof(buf));

                    recv(i, buf, MAX, 0);
                    printf("Message from client: %s\n", buf);

                    printf("Server: ");
                    //scanf("%[^\n]", msg);
                    int j = 0; while((msg[j++] = getchar()) != '\n');
                    send(i, msg, MAX, 0);
                }
            }
        }
    }

    return 0;
}
