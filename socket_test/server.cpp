#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 8080

void error(const char *msg);
int setup_server(int server_port);
int accept_new_connection(int server_socket);


int main(int, char**)
{
    int server_socket = setup_server(SERVER_PORT);

    while(true)
    {
        printf("WAITING FOR CONNECTIONS");

        int client_socket = accept_new_connection(server_socket);
    }

    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int setup_server(int server_port)
{
    int socketfd;
    struct sockaddr_in serv_addr;

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error("UNABLE TO CREATE A SOCKET");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(server_port);

    if(bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("UNABLE TO BIND A SOCKET");
    }

    listen(socketfd, 5);

    return socketfd;
}

int accept_new_connection(int server_socket)
{
    int newsockfd, valread;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    char buffer[1024] = {0};

    clilen = sizeof(cli_addr);

    newsockfd = accept(server_socket, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);
    if(newsockfd < 0) 
    {
        error("UNABLE TO ACCEPT A CONNECTION\n");
    }

    printf("ESTABLISHED A NEW CONNECTION\n");

    valread = read(newsockfd, buffer, 1024);

    printf("%s\n", buffer);

    return newsockfd;
}

