#include "opencv2/opencv.hpp"
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int, char**)
{

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
    // imwrite("alpha.png", frame); ta linia zapisuje image w folderze, moze byc potrzebne w przyszlosci
    if(waitKey(30) >= 0) break;
}

return 0;
}