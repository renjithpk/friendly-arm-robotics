#include "types.h"
#include "thread.h"
#include "tcpsocket.h"
#include "cvInterface.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <X11/Xlib.h> 
using namespace std;

string ip_addr="";// = argv[1];

//pthread_mutex_t gui_mtx = PTHREAD_MUTEX_INITIALIZER;

void tSwaitKey(int time)
{
//	if(0 == pthread_mutex_trylock(&gui_mtx))
//	{
		waitKey(time);
//		pthread_mutex_unlock(&gui_mtx);
//	}
}

//Mat streamImg;
pthread_mutex_t img_mtx = PTHREAD_MUTEX_INITIALIZER;
Mat vcImage;
cv::Size image_size;

pthread_mutex_t circle_mtx = PTHREAD_MUTEX_INITIALIZER;

enum Enum_ObjState
{
	OBJ_STATE_INIT = 0,
	OBJ_STATE_DETECTED, 
	//		OBJ_STATE_CONFIRED,  
	//		OBJ_STATE_M_CONFIRMED,
	OBJ_STATE_M_DETECTED,
	OBJ_STATE_NONE
};
static Enum_ObjState state = OBJ_STATE_INIT;


void copyRCircle(RCircle & c,Vec3f circleVec)
{
			c.x = cvRound(circleVec[0]);
			c.y = cvRound(circleVec[1]);
			c.r = cvRound(circleVec[2]);
}

class CaptureImage:public Thread
{

	static VideoCapture vcap;
	static RCircle rCircle;//x; y;r; maxx; maxy;count;
	public:

		void Execute(void*)
		{

			pthread_mutex_lock(&img_mtx);
			string streamAdd = "http://";
			if(!ip_addr.empty())
			{
				streamAdd = streamAdd + ip_addr;//"192.168.0.102"/*ip*/;
				streamAdd = streamAdd + ":8080/?action=stream&amp;type=.mjpg";
			}
			while(1)
			{
				if(!ip_addr.empty())
				{
					cout<<"stream url"<<streamAdd<<endl;
					vcap.open(streamAdd);
				}
				else
				{
					cout<<"stream address not mentioned "<<endl;
					vcap.open(0);
				}
				if (!vcap.isOpened())
				{
					cout<<"failed to open stream or camera \n retry...."<<endl;
					sleep(10);
				}
				else
				{
					break;	
				}
			}
			Mat img;
			vcap.grab();
			vcap.retrieve(img);
			image_size = img.size();
			pthread_mutex_unlock(&img_mtx);
			while(1)
			{

				pthread_mutex_lock(&img_mtx);
				vcap.grab();
				pthread_mutex_unlock(&img_mtx);
				waitKey(10);
			}
		}
		static void readImage(Mat &img)
		{
			pthread_mutex_lock(&img_mtx);
			vcap.retrieve(img);
			pthread_mutex_unlock(&img_mtx);

		}
		static void getRCircle(RCircle & rcircle)
		{
			pthread_mutex_lock(&circle_mtx);
			rcircle = rCircle;	
			pthread_mutex_unlock(&circle_mtx);
		}
		static void setRCircle(const RCircle & rcircle)
		{
			pthread_mutex_lock(&circle_mtx);
			rCircle =rcircle;	
			pthread_mutex_unlock(&circle_mtx);
		}
};

RCircle CaptureImage::rCircle;//x; y;r; maxx; maxy;count;
VideoCapture CaptureImage::vcap;



//-------value--------- 
// ---loL------upL------
// return 0 if equal to ref
// return -1 if small than ref
// return 1 if larger than ref

int compare(int value,int ref, int err)
{
	int upL = ref + err;
	int loL = ref - err;
	if( value < loL)
	{
		return -1;	
	}
	if(value > upL)
	{
		return 1;
	}
	return 0;
}

class BallDetector:public Thread
{
	public:
	cvIf cv;

	void Execute(void*)
	{
		//pthread_mutex_lock(&gui_mtx);
		cv.createBallTracker();
		//pthread_mutex_unlock(&gui_mtx);

		//waitKey(50);
		while(1)
		{
			//sleep(1);
			int count = detectBall();
			//waitKey(50);
			//cout<<"."<<flush;
		}
	}

	int detectBall()
	{
		int countBall = 0;
		static RCircle trCircle;
		vector<Vec3f> circles;
		Mat copiedImg;
		static std::stringstream sstm;
		CaptureImage::readImage(copiedImg);
		countBall = cv.detectBall(circles,copiedImg);
		
		int nonZeros =cv.getNzOfLast();
		if(countBall > 0)
		{
			sstm.str("");
			copyRCircle(trCircle, circles[0]);
			objectState(countBall,trCircle);
			sstm<<image_size.width<<"x"<<image_size.height<<"   [x : y : r] ["<<trCircle.x<<" : "<<
				trCircle.y<<" : "<<trCircle.r<<"]    "<<(int)state<<" nz "<<nonZeros;
			
			//pthread_mutex_lock(&gui_mtx);
			cv.displayImgWithCircle(copiedImg,circles,sstm.str());
			//pthread_mutex_unlock(&gui_mtx);
		}
		else
		{
			objectState(countBall);
			//pthread_mutex_lock(&gui_mtx);
			sstm<<image_size.width<<"x"<<image_size.height<<"   [x : y : r] ["<<trCircle.x<<" : "<<
				trCircle.y<<" : "<<trCircle.r<<"]    "<<(int)state<<" nz "<<nonZeros;
			cv.displayImgage(copiedImg,sstm.str());
			//pthread_mutex_unlock(&gui_mtx);
		}
		return countBall;
	}


	void objectState(int noOfCircle, RCircle rcircle = 0)
	{
		static int count = 0;
		switch(state)
		{
			case OBJ_STATE_INIT:
				if(noOfCircle == 0)
				{
					state = OBJ_STATE_NONE;
				}else if(noOfCircle == 1)
				{
					state = OBJ_STATE_DETECTED;
					CaptureImage::setRCircle(rcircle);
					//store RCircle
				}
				else
				{
					state = OBJ_STATE_M_DETECTED;	
				}
				break;
			case OBJ_STATE_DETECTED:
				if(noOfCircle == 0)
				{
					state = OBJ_STATE_NONE;
					count = 0;
				}else if(noOfCircle == 1)
				{
					//store RCircle
					CaptureImage::setRCircle(rcircle);
					count = 0;
				}
				else
				{
					state = OBJ_STATE_M_DETECTED;
					count = 0;
				}
			break;
			case OBJ_STATE_M_DETECTED:
				if(noOfCircle == 0)
				{
					state = OBJ_STATE_NONE;
					count = 0;
				}else if(noOfCircle == 1)
				{
					state = OBJ_STATE_DETECTED;
					//store RCircle
					CaptureImage::setRCircle(rcircle);
					count = 0;
				}
				else
				{
					if(count > 5)
					{
						//store Multiple object 	
					}
					else
						count++;
				}

				break;
			case OBJ_STATE_NONE:

				if(noOfCircle == 0)
				{
					if(count > 5)
					{
						//STORE NONE
					}
					else
						count++;
				}else if(noOfCircle == 1)
				{
					state = OBJ_STATE_DETECTED;
					//store RCircle
					count = 0;
				}
				else
				{
					state = OBJ_STATE_M_DETECTED;
					count = 0;
				}			
				break;
		}	
	}

};

class MyApp:public SocketIf
{
public:
	Socket socket;

	int startme(void)
	{
		CaptureImage cimage;
		cimage.Start();
		BallDetector bd;
		bd.Start();
		socket.registerInterface(this);
		if(-1 == socket.initServer(5000)) return -1;
		
		cout<< "Waiting for client"<<endl;
		if ( ! socket.accept ( ) )
		{
			cout<< "Could not accept socket."<<endl;
			return -1;
		}

		
		Epoll::getInstance()->run();
		cout<<"client connected"<<endl;
	}

	int onDataReceived(unsigned char * buffer, int size)
	{
		cout<<"rserver socket data received"<<endl;

		Header *header_c = (Header *)buffer;
		if(header_c->type == RQ_OBJ_DETECT)
		{	
			cout<<"Request for face detection"<<endl;
			MSG_Circle msg;
			switch(state)
			{
				case OBJ_STATE_INIT:
				case OBJ_STATE_NONE:
					msg.header.type = DTD_OBJ_NOBALL;
					socket.send((unsigned char *)&msg,sizeof(MSG_Circle));

					break;
				case OBJ_STATE_DETECTED: 
					msg.header.type = DTD_OBJ_BALL;
					
					CaptureImage::getRCircle(msg.rCircle);
					socket.send((unsigned char *)&msg,sizeof(MSG_Circle));

					break;
				case OBJ_STATE_M_DETECTED:
					msg.header.type = DTD_OBJ_M_BALL;
					socket.send((unsigned char *)&msg,sizeof(MSG_Circle));	
					break;

			}
		}
	}

	int clientDisconnected()
	{
		cout<< "Waiting for client"<<endl;
		if ( ! socket.accept ( ) )
		{
			cout<< "Could not accept socket."<<endl;
			return -1;
		}	
		cout<< "client connected"<<endl;
	}



#if 0
    int detectFace(Rect_<int> &rect)
	{
		int ret = 0;
		vector< Rect_<int> > faces;
		Mat img;
		cv.getCapture()>> img;
		int nos = cv.detectFace(img,faces);
		if(nos > 0)
		{
			ret = 1;
			rect = faces[0];
			cv.displayImageWithRect(img,rect);
		
		}else
		{
			cv.displayImageWithRect(img);
		}

		waitKey(50);	
		return ret;
	}
#endif

};


int main(int argc,char * argv[])
{
	MyApp myApp;
	XInitThreads();	
	if(argc == 2)
	{
		ip_addr = argv[1];
	}

	myApp.startme();
}
