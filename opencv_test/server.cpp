#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

#define PORT 8080

int main(int, char**)
{

    // tworzymy nowy socket
    int server_fd, new_socket, valread;

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[700000] = {0};

    // tworzymy file descriptor
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("nie mozna utworzyc socketu");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("nie udalo sie polaczyc socketa funkcja bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0)
    {
        perror("blad podczas nasluchiwania");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // kod zwiazany z opencv
    // tworzymy obiekt cap
    VideoCapture cap;

    // otwieramy plik sample.mp4
    cap.open("sample.mp4");

    // tworzymy nowe okno i nazywamy je 'Video'
    namedWindow("Video",1);
    for(;;)
    {
        Mat frame;
        cap >> frame;    
        imshow("Video", frame);

        // obliczamy jak duza jest jedna klatka
        int imgSize = frame.total() * frame.elemSize();
        //std::cout << cap.get(3) << cap.get(4) << std::endl;

        // wysylamy frame do klienta
        int bytes = send(new_socket, frame.data, imgSize, 0);

        // imwrite("alpha.png", frame); ta linia zapisuje image w folderze, moze byc potrzebne w przyszlosci
        
        if(waitKey(30) >= 0) break;
    }
    return 0;
}