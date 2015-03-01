#include "cvInterface.h"
#include <iostream>

using namespace std;



cvIf::~cvIf()
{
	//delete haar_cascade_p;
}
int cvIf::createBallTracker()
{
	return tracker.create();	
}
int cvIf::detectBall(vector<Vec3f> &circles,Mat &imgOriginal)
{
	Mat imgHSV;
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	//imgHSV = imgOriginal;
	Mat imgThresholded;

	inRange(imgHSV, Scalar(tracker.mLowH, tracker.mLowS, tracker.mLowV), Scalar(tracker.mHighH, tracker.mHighS, tracker.mHighV), imgThresholded); //Threshold the image

	//morphological opening (remove small objects from the foreground)
	//erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	//dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (fill small holes in the foreground)
	//dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	//erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));	

	tracker.displayImgage(imgThresholded);
	Mat imgThresholded2;
	bilateralFilter(imgThresholded, imgThresholded2, 15, 1000, 1000);
	

	/// Apply the Hough Transform to find the circles
//	cout<<"start "<<time(0)<<endl;
	HoughCircles(imgThresholded2, circles, CV_HOUGH_GRADIENT, 1,imgThresholded2.rows/2,tracker.mThrEd, tracker.mThrCd,tracker.mMinR);
//	cout<<"end "<<time(0)<<endl;

	return circles.size();
}

int cvIf::displayImgage(Mat &img,const string &str)
{
	if(false == isWindowOpened)
	{		
		isWindowOpened = true;
		namedWindow("Robo", CV_WINDOW_NORMAL);
	}
	if(!str.empty())
	{
			putText(img, str,
					cvPoint(30,30),  //text origin
					FONT_HERSHEY_COMPLEX_SMALL, //font type
					0.8, 						//font scale
					cvScalar(255,0,0),	
					1, //thickness
					CV_AA);
	}
	imshow("Robo", img);
	waitKey(30);
}


int cvIf::displayImageWithRect(Mat &img,const Rect_<int> &rect)
{
	if(false == isWindowOpened)
	{		
		isWindowOpened = true;
		namedWindow("Robo", CV_WINDOW_NORMAL);
	}

	cv::rectangle(
			img,
			/*cv::Point(5, 10),
			  cv::Point(20, 30),*/
			rect,
			cv::Scalar(255, 255, 255),
			10
			);
	//cout<<"size  "<<faces[i].width*faces[i].height<<" x "<<faces[i].x<<" y"<<faces[i].y<<endl;
	imshow("Robo", img);
	waitKey(30);
}

int cvIf::displayImgWithCircle(Mat &image, vector<Vec3f> &circles,const string &str)
{
	if(false == isWindowOpened)
	{		
		isWindowOpened = true;
		namedWindow("Robo", CV_WINDOW_AUTOSIZE);
	}
	if(!str.empty())
	{
			putText(image, str,
					cvPoint(30,30),  //text origin
					FONT_HERSHEY_COMPLEX_SMALL, //font type
					0.8, 						//font scale
					cvScalar(0,255,0),	
					1, //thickness
					CV_AA);
	}
	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(image, center, 3, Scalar(0,255,0), -1, 8, 0 );
		circle(image, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}
	imshow("Robo",image);
	waitKey(30);
	return 0;
}
int cvIf::setCascadeFile(const string &file)
{
	cascadeFile_m = file;
}

int cvIf::detectFace(Mat &img, vector< Rect_<int> > &faces)
{
	if(!isHaarLoaded)
	{
		cout<<"loading haarcascade_frontalface_alt.xml "<<endl;
		haar_cascade.load(cascadeFile_m);
		isHaarLoaded = true;
	}

	// Find the faces in the frame:
	haar_cascade.detectMultiScale(img, faces);	
	return faces.size();
}


cvIf::cvIf(const char *ip)
{
	enableCam = true;
	isWindowOpened = false;
	isHaarLoaded = false;
//	grayMat_m = Mat::zeros(480,640,CV_8UC1);
//	imgSize = grayMat_m.total()*grayMat_m.elemSize();
	string streamAdd = "http://";
	streamAdd = streamAdd + ip;
	streamAdd = streamAdd + ":8080/?action=stream&amp;type=.mjpg";
	if(enableCam)
	{
		capture.open(streamAdd);
		if (!capture.isOpened())
		{
			cout<<"failed to open camera"<<endl;
			enableCam = false;
		}
	}
}

cvIf::cvIf(bool camera)
{
	enableCam 		= camera;
	isWindowOpened 	= false;
	isHaarLoaded 	= false;
//	grayMat_m = Mat::zeros(480,640,CV_8UC1);
//	imgSize = grayMat_m.total()*grayMat_m.elemSize();
	if(enableCam)
	{
		capture.open(0);
		if (!capture.isOpened())
		{
			cout<<"failed to open camera"<<endl;
			enableCam = false;
		}
	}
}

int cvIf::getImgSize()
{
	return imgSize;
}


VideoCapture& cvIf::getCapture()
{	
	return capture;
}

Mat * cvIf::getImage()
{
	Mat cImg;
	if(!enableCam)
	{
		cout<<"camera not enabled"<<endl;
		return NULL;
	}
	//capture >> cImg;
	for(int i=0;i<10 && !capture.grab();i++)
	{
		sleep(1);
		cout<<"x"<<flush;
		if(i == 10) return NULL;
	}
	for(int i=0;i<20 && !capture.retrieve(cImg);i++)
	{
		cout<<"z"<<flush;
		capture.grab();
		sleep(1);
		if(i == 20) return NULL;
	}

	flip(cImg, cImg, 1);
	cvtColor(cImg, grayMat_m, CV_BGR2GRAY);		
	return &grayMat_m;
}

int cvIf::imageFromData(Mat &img,unsigned char * data)
{

		for (int i = 0;  i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				(img.row(i)).col(j) = (uchar)data[((img.cols)*i)+j];
			}
		}

}
/*
int cvIf::convertImage(Mat &img)
{

}*/









