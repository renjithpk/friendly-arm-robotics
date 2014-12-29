#include "cvInterface.h"

int cvIf::displayImgage(Mat &img)
{
	if(false == isWindowOpened)
	{		
		isWindowOpened = true;
		namedWindow("cv_if", CV_WINDOW_AUTOSIZE);
	}
	imshow("cv_if", img);

}

cvIf::~cvIf()
{
	//delete haar_cascade_p;
}

int cvIf::displayImgage(Mat &img,Rect_<int> &rect)
{
	if(false == isWindowOpened)
	{		
		isWindowOpened = true;
		namedWindow("cv_if", CV_WINDOW_AUTOSIZE);
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
	imshow("cv_if", img);

}


int cvIf::detectFace(Mat &img, vector< Rect_<int> > &faces)
{
	if(!isHaarLoaded)
	{
		cout<<"loading haarcascade_frontalface_alt.xml "<<endl;
		haar_cascade.load("haarcascade_frontalface_alt.xml");
		isHaarLoaded = true;
	}

	// Find the faces in the frame:
	haar_cascade.detectMultiScale(img, faces);		
}


cvIf::cvIf(bool camera)
{
	enableCam = camera;
	isWindowOpened = false;
	isHaarLoaded = false;
	grayMat_m = Mat::zeros(480,640,CV_8UC1);
	imgSize = grayMat_m.total()*grayMat_m.elemSize();
	if(enableCam)
	{
		cout<<"camera opened"<<endl;
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









