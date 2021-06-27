#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <string>
using namespace std;
using namespace cv;

class detect_object
{
      private:

      public:
   
  detect_object(void)
  {
        system("mplayer Hello.mp2");
  }

 ~detect_object(void)
  {
	destroyAllWindows();
  }
     
  static bool compareContourAreas ( vector<Point> contour1, vector<Point> contour2 )
  {
      double i = fabs( contourArea(Mat(contour1)) );
      double j = fabs( contourArea(Mat(contour2)) );
    
      return ( i > j );
  }

  void init()
  {
      
  }

  Mat detect_sign_and_draw( Mat frame, Scalar lower, Scalar upper )
  {
      return frame;  	   
  }

  Mat detect_light_and_draw( Mat frame, Scalar lower, Scalar upper )
  {
     return frame;
  }


};

int main()
{
    startWindowThread();
     
    Mat frame;
     
    VideoCapture cap;
    cap.open(0);
    
    detect_object puma = detect_object();
    puma.init();

    while (cap.read(frame))
    {
           cap >> frame;
           
	   frame = puma.detect_sign_and_draw(frame, Scalar(100, 85, 130), Scalar(255, 255, 255));
           frame = puma.detect_light_and_draw(frame, Scalar(15, 80, 100), Scalar(38, 210, 255));

	   imshow("Frame", frame);
           if (waitKey(1) == 27) break;

    }

 return 0;
}
