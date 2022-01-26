#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 8080

void error(const char *msg);
int create_connection();

int main(int, char**)
{
    int socketfd = create_connection();

    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int create_connection()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char msg[] = "Hello from client!";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("ERROR WITH CREATING A SOCKET");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        error("INVALID ADDRESS");
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("UNABLE TO MAKE A CONNECTION\n");
    }

    send(sockfd, msg, strlen(msg), 0);

    return sockfd;
}
