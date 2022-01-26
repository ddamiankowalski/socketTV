#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include <stdio.h>
#include <arpa/inet.h>
#include <vector>

#define PORT 8080

using namespace cv;

int main(int, char**)
{
    int sock = 0, valread, bytes;

    struct sockaddr_in serv_addr;
    char buffer[700000] = {0};
    uchar sockData[691200];
    
    // tworzymy socket po stronie klienta
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("problem podczas tworzenia socketu");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("problem podczas pton");
        exit(EXIT_FAILURE);
    }

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("nie mozna polaczyc sie z serwerem");
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "prawidlowo polaczono sie z serwerem!" << std::endl;
    }

    namedWindow("Video2",1);

    for(;;)
    {
        // valread = read(sock, buffer, 700000);
        // std::cout << valread << std::endl;

        for (int i = 0; i < 691200; i += bytes) {
            if ((bytes = recv(sock, sockData +i, 691200  - i, 0)) == -1)
                perror("nie udalo sie przeslac ramki");
        }


        // tworzymy obiekt frame u klienta
        Mat img(Size(640, 360), CV_8UC3, sockData);

        // pokazujemy framea
        imshow("Video2", img);

        if(waitKey(30) >= 0) break;
    }
    return 0;
}