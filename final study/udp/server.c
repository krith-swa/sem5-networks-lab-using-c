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

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    fd_set read;
    int max;
    FD_ZERO(&read);
    max = sockfd;

    while(1) {
        FD_SET(sockfd, &read);

        if (select(max + 1, &read, NULL, NULL, NULL) < 0) {
            printf("select error\n");
            exit(0);
        }
        printf("select\n");

        if (FD_ISSET(sockfd, &read)) {
            bzero(msg, sizeof(msg));
            bzero(buf, sizeof(buf));

            int len = sizeof(cli);
            recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &cli, &len);
            printf("Message from client %d: %s\n", cli.sin_addr, buf);

            printf("Server: ");
            //scanf("%[^\n]", msg);
            int j = 0; while((msg[j++] = getchar()) != '\n');
            sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr*) &cli, len);
        }
    }

    return 0;
}