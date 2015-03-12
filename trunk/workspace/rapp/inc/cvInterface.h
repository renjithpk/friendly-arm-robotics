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
	Tracker();
	int create();
	int displayImgage(Mat &img);
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
	Mat imgThresholded;
	public:
	~cvIf();
	cvIf(bool camera = false);
	cvIf(const char *ip);
	Mat * getImage();
	//int convertImage(Mat &img);
	int getImgSize();
	int displayImgage(Mat &img,const string &str = "");
	int displayImageWithRect(Mat &img,const Rect_<int> &rect);
	int displayImgWithCircle(Mat &img,vector<Vec3f> &circles,const string &str = "");
	int imageFromData(Mat &img,unsigned char * data);
	int setCascadeFile(const string &file);
	int detectFace(Mat &img,vector< Rect_<int> > &faces);
	int createBallTracker();
	int detectBall(vector<Vec3f> &circles, Mat &imgOriginal);
	int getNzOfLast();
	VideoCapture& getCapture();

};

#endif
