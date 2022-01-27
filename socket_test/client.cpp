#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#define PORT 8081

using namespace cv;

void error(const char *msg);
int create_connection();
int download_frame(int servsockfd);
void send_confirmation(char const *msg, int socketfd);
Mat receive_frame(int sockfd);

int main(int, char**)
{
    int valread;
    int socketfd = create_connection();
    char buffer[1024] = {0};

    Mat img = imread("sample.jpeg", IMREAD_COLOR);
    namedWindow("Client",1);

    for(;;)
    {
        imshow("Client", img);

        if(waitKey(30) >= 0) break;

        // wysylamy informacje ze jestesmy gotowi na kolejna ramke
        send_confirmation("ready", socketfd);

        // czytamy kolejna ramke tutaj
        img = receive_frame(socketfd);

        // zapisujemy ramke do mat i wyswietlamy
    }

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

void send_confirmation(char const *msg, int socketfd)
{
    send(socketfd, msg, strlen(msg), 0);
}

Mat receive_frame(int sockfd)
{
    Mat img = Mat::zeros(360, 640, CV_8UC3);
    int imgSize = img.total() * img.elemSize();
    uchar sockData[imgSize];
    int bytes = 0;

    for (int i = 0; i < imgSize; i += bytes) {
        if ((bytes = recv(sockfd, sockData +i, imgSize  - i, 0)) == -1)  {
            error("FRAME SENDING FAILED");
        }
    }

    int ptr=0;
    for (int i = 0;  i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {                                     
            img.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
            ptr=ptr+3;
        }
    }

    return img;
}