#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
VideoCapture cap;
cap.open("sample.mp4");

namedWindow("Video",1);
for(;;)
{
    Mat frame;
    cap >> frame; // get a new frame from camera        
    imshow("Video", frame);
    if(waitKey(30) >= 0) break;
}
// the camera will be deinitialized automatically in VideoCapture destructor
return 0;
}