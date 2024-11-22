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

    if (connect(sockfd, (struct sockaddr*) &serv, sizeof(serv)) < 0) {
        printf("connect error\n");
        exit(0);
    }
    printf("connect\n");

    while(1) {
        bzero(&msg, sizeof(msg));
        bzero(&buf, sizeof(buf));

        printf("Client: ");
        //scanf("%[^\n]", msg);
        int j = 0; while((msg[j++] = getchar()) != '\n');
        send(sockfd, msg, MAX, 0);

        recv(sockfd, buf, MAX, 0);
        printf("Message from server: %s\n", buf);
    }

    return 0;
}
