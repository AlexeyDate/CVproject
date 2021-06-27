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
	     time_t now;
   	     time_t timePedestrian;
   	     time_t timeTurnRight;
   	     time_t timeTurnLeft;

   	     bool flagPedestrian = true;
 	     bool flagTurnRight = true;
	     bool flagTurnLeft = true;
 
         bool flag = false;

	     int countPedestrian = 0;
	     int countTurnRight = 0;
  	     int countTurnLeft = 0;

  	     int redSum = 0;
   	     int greenSum = 0;
	     int yellowSum = 0;

	     CascadeClassifier detect_light;

         Mat Pedestrian = imread( "Pedestrian.png" );
     	 Mat TurnRight = imread( "TurnRight.png" );
 	     Mat TurnLeft = imread( "TurnLeft.jpg" );

    	 Mat frameCopy;
  	     Mat frameHSV;
         Mat img;
         Mat frameLights;
         Mat imgLights;
      	 vector<vector<Point>> contours;
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
      resize(Pedestrian, Pedestrian, Size (64, 64));
      resize(TurnRight, TurnRight, Size (64, 64));
      resize(TurnLeft, TurnLeft, Size (64, 64));
   
      cvtColor(Pedestrian, Pedestrian, COLOR_BGR2HSV);
      inRange(Pedestrian, Scalar(100, 180, 180), Scalar(255, 255, 255), Pedestrian);
   
      cvtColor(TurnRight, TurnRight, COLOR_BGR2HSV);
      inRange(TurnRight, Scalar(50, 0, 0), Scalar(255, 255, 255), TurnRight);
   
      cvtColor(TurnLeft, TurnLeft, COLOR_BGR2HSV);
      inRange(TurnLeft, Scalar(0, 140, 0), Scalar(255, 255, 255), TurnLeft);
  }

  Mat detect_sign_and_draw( Mat frame, Scalar lower, Scalar upper )
  {
       contours.clear();
       frame.copyTo(frameCopy);
             
       cvtColor(frame, frameHSV, COLOR_BGR2HSV);
       blur(frameHSV, frameHSV, Size (3,3));
       inRange(frameHSV, lower, upper, frameHSV);
       
       erode(frameHSV, frameHSV, 2);
       dilate(frameHSV, frameHSV, 4);

       findContours(frameHSV, contours, RETR_TREE, CHAIN_APPROX_NONE); 
       
       if (!contours.empty())
       {
           countPedestrian = 0;
           countTurnRight = 0;
           countTurnLeft = 0;
           
		   now = time(NULL);

           sort(contours.begin(), contours.end(), compareContourAreas);
           drawContours(frame, contours, 0, Scalar (255, 0, 255), 3);
           Rect rect = boundingRect(contours[0]);	 
		   rectangle(frame, rect, Scalar(0, 255, 0), 2);
		   img = frameCopy(rect);
				
           if (rect.size().height >= 64 && rect.size().width >= 64) 
		   {
				resize(img, img, Size (64, 64));
				cvtColor(img, img, COLOR_BGR2HSV);
				inRange(img, lower, upper, img);
           
				for (int i = 0; i < img.size().height; i++)
				{
						Vec3b* ptrImg = img.ptr<Vec3b>(i);
						Vec3b* ptrPedestrian = Pedestrian.ptr<Vec3b>(i);
						Vec3b* ptrTurnRight = TurnRight.ptr<Vec3b>(i);
						Vec3b* ptrTurnLeft = TurnLeft.ptr<Vec3b>(i);
		 
					for (int j = 0; j < img.size().width; j++)
					{  
							if (ptrImg[j] == ptrPedestrian[j]) countPedestrian += 1;	  
							if (ptrImg[j] == ptrTurnRight[j]) countTurnRight += 1;	  
							if (ptrImg[j] == ptrTurnLeft[j]) countTurnLeft += 1;	  
					}
				}
           
             /*
	   	cout << "NoDrive:  " << countNoDrive << "       "
		     << "GiveWay:  " << countGiveWay << "       "
		     << "Pedestrian:  " << countPedestrian << "       "
		     << "TurnRight:  " << countTurnRight << "       "
		     << "TurnLeft:  " << countTurnLeft << "       "
	       	     << "Attention:  " << countAttention << "      " << endl;
               */

				if (countPedestrian > 3000 && flagPedestrian)
				{
					cout << "Detect Pedestrian!" << endl;
					system("mplayer Pedestrian.mp2");
					timePedestrian = time(NULL);
					flagPedestrian = false;
				}
	   	
				if (countTurnRight > 3000 && flagTurnRight)
				{
					cout << "Detect TurnRight!" << endl;
					system("mplayer TurnRight.mp2");
					timeTurnRight = time(NULL);
					flagTurnRight = false;
				}
	   	
				if (countTurnLeft > 3000 && flagTurnLeft)
				{
					cout << "Detect TurnLeft!" << endl;
					system("mplayer TurnLeft.mp2");
					timeTurnLeft = time(NULL);
					flagTurnLeft = false;
				}

				if (now - timePedestrian > 9) flagPedestrian = true;
				if (now - timeTurnRight > 9) flagTurnRight = true;
				if (now - timeTurnLeft > 9) flagTurnLeft = true;
			}
		}

      return frame;  	   
  }

  Mat detect_light_and_draw( Mat frame, Scalar lower, Scalar upper )
  {
     contours.clear();
     
     frame.copyTo(frameCopy);
	   
     cvtColor(frame, frameHSV, COLOR_BGR2HSV);
     blur(frameHSV, frameHSV, Size (3,3));
     inRange(frameHSV, lower, upper, frameHSV);
     
     erode(frameHSV, frameHSV, 2);
     dilate(frameHSV, frameHSV, 4);

     findContours(frameHSV, contours, RETR_TREE, CHAIN_APPROX_NONE); 
     
     if (!contours.empty())  
     {
         sort(contours.begin(), contours.end(), compareContourAreas);
         drawContours(frame, contours, 0, Scalar (255, 100, 100), 3);
         Rect rect = boundingRect(contours[0]);	 
	     rectangle(frame, rect, Scalar(0, 255, 0), 2);
	     img = frameCopy(rect);

         if (rect.size().height > 200 && rect.size().width > 70 && rect.size().height - rect.size().width > 130)
         {
             redSum = 0;
             greenSum = 0;
             yellowSum = 0;    
                        
             cvtColor(img, img, COLOR_BGR2HSV);
             resize(img, img, Size(40, 120));

             rectangle(img, Size (0,0), Size (40,40), Scalar (0, 0, 255), 2);
             rectangle(img, Size (0,40), Size (40,80), Scalar (0, 255, 255), 2);
             rectangle(img, Size (0,80), Size (40,120), Scalar (0, 255, 0), 2);
                
             for (int k = 0; k < img.size().height; k++ )
             {
                   for (int j = 0; j < img.size().width; j++)
                   {
                         if (k < 40) redSum += img.at<Vec3b>(k,j)[2];           
                         if (k > 40 && k < 80) yellowSum += img.at<Vec3b>(k,j)[2];      
                         if (k > 80) greenSum += img.at<Vec3b>(k,j)[2];         
                   }       
             }

	         cout << "redSum:  " << redSum << "      "
                 << "yellowSum:  " << yellowSum << "      "
                 << "greenSum:  " << greenSum << endl;
	       
         }
     }

     imshow("Light", img);

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