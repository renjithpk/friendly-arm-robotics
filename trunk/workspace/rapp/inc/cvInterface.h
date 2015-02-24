#ifndef CV_INTERFACE_H_
#define CV_INTERFACE_H_
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "opencv2/objdetect/objdetect.hpp"

#include "types.h"
#include "tcpsocket.h"
using namespace cv;
using namespace std;



class Tracker
{
	const char *name;
public:
	int mLowH;
	int mHighH;
	int mLowS; 
	int mHighS;
	int mLowV;
	int mHighV;
	int mThrEd;
	int mThrCd;
	int mMinR;
	Tracker():name("Controller")
	{
		mLowH = 43;
		mHighH = 86;
		mLowS = 102; 
		mHighS = 167;
		mLowV = 93;
		mHighV = 148;
		mThrEd = 29;
		mThrCd = 32;
		mMinR = 20;
	}

	int create()
	{
		namedWindow(name, CV_WINDOW_AUTOSIZE); //create a window called "Control"
		//Creattrackers
		cvCreateTrackbar("LowH", name, &mLowH, 179); //Hue (0 - 179)
		cvCreateTrackbar("HighH", name, &mHighH, 179);

		cvCreateTrackbar("LowS", name, &mLowS, 255); //Saturation (0 - 255)
		cvCreateTrackbar("HighS", name, &mHighS, 255);

		cvCreateTrackbar("LowV", name, &mLowV, 255); //Value (0 - 255)
		cvCreateTrackbar("HighV", name, &mHighV, 255);

		cvCreateTrackbar("ThrEd", name, &mThrEd, 255); //Value (0 - 255)
		cvCreateTrackbar("ThrCd", name, &mThrCd, 255);
		cvCreateTrackbar("minR", name, &mMinR, 255);
		return 0;
	}
	int displayImgage(Mat &img)
	{
		imshow(name, img);
		return 0;
	}
};



class cvIf
{
	bool enableCam;
	bool isWindowOpened;
	VideoCapture capture;
	Mat grayMat_m;
	int imgSize;
	CascadeClassifier haar_cascade;
	bool isHaarLoaded;
	Tracker tracker;
	//CascadeClassifier *haar_cascade_p;
	string cascadeFile_m;	
	public:
	~cvIf();
	cvIf(bool camera = false);
	cvIf(const char *ip);
	Mat * getImage();
	//int convertImage(Mat &img);
	int getImgSize();
	int displayImgage(Mat &img);
	int displayImgage(Mat &img,const Rect_<int> &rect);
	int drawCircle(Mat &img,vector<Vec3f> &circles);
	int imageFromData(Mat &img,unsigned char * data);
	int setCascadeFile(const string &file);
	int detectFace(Mat &img,vector< Rect_<int> > &faces);
	int createBallTracker();
	int detectBall(vector<Vec3f> &circles, Mat &imgOriginal);
	VideoCapture& getCapture();

};

#endif
