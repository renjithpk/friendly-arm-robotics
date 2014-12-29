#include "my_socket.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "opencv2/objdetect/objdetect.hpp"
using namespace cv;




class cvIf
{
	bool enableCam;
	bool isWindowOpened;
	VideoCapture capture;
	Mat grayMat_m;
	int imgSize;
	CascadeClassifier haar_cascade;
	bool isHaarLoaded;
	//CascadeClassifier *haar_cascade_p;
		
	public:
	~cvIf();
	cvIf(bool camera = false);
	Mat * getImage();
	//int convertImage(Mat &img);
	int getImgSize();
	int displayImgage(Mat &img);
	int displayImgage(Mat &img,Rect_<int> &rect);
	int detectFace(Mat &img,vector< Rect_<int> > &faces);
	int imageFromData(Mat &img,unsigned char * data);

};
