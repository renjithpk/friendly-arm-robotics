#include "types.h"
#include "thread.h"
#include "tcpsocket.h"
#include "rtimer.h"
#include "cvInterface.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <X11/Xlib.h> 
using namespace std;

string ip_addr="";

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
	OBJ_STATE_M_DETECTED_C,
	OBJ_STATE_NONE,
	OBJ_STATE_NONE_C
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

class BallDetector:public RTimer
{
	public:
	BallDetector(Socket & socket):socket_p(&socket)
	{}

	BallDetector()
	{}

	cvIf cv;
	RCircle trCircle;
	std::stringstream sstm;
	MSG_Circle rspMsg;
	Socket *socket_p;
	
	int startRepeatedDetect(Socket &socket)
	{
		socket_p =&socket;
		setDelayRep(1);	
	}

	void onTimerExpired()
	{
		if(detectBall() > 0)
		{
			cout<<"."<<flush;
			rspMsg.header.type = DTD_OBJ_BALL;
			rspMsg.rCircle =  trCircle;
			socket_p->send((unsigned char *)&rspMsg,sizeof(MSG_Circle));
		}
	}
	int stopRepeatDetect()
	{
		stopTimer();	
	}
	int detectBall()
	{
		int countBall = 0;
		vector<Vec3f> circles;
		Mat copiedImg;
		CaptureImage::readImage(copiedImg);
		countBall = cv.detectBall(circles,copiedImg);
		
		int nonZeros =0;//cv.getNzOfLast();
		if(countBall > 0)
		{
			sstm.str("");
			copyRCircle(trCircle, circles[0]);
			sstm<<image_size.width<<"x"<<image_size.height<<"   [x : y : r] ["<<trCircle.x<<" : "<<
				trCircle.y<<" : "<<trCircle.r<<"]    "<<(int)state<<" nz "<<nonZeros;
			
			cv.displayImgWithCircle(copiedImg,circles,sstm.str());
		}
		else
		{
			sstm<<image_size.width<<"x"<<image_size.height<<"   [x : y : r] ["<<trCircle.x<<" : "<<
				trCircle.y<<" : "<<trCircle.r<<"]    "<<(int)state<<" nz "<<nonZeros;
			cv.displayImgage(copiedImg,sstm.str());
		}
		return countBall;
	}


};

class MyApp:public SocketIf 
{
public:
	Socket socket;
	BallDetector bd;

	int startme(void)
	{
		CaptureImage cimage;
		cimage.Start();
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

		switch(header_c->type)
		{
			case RQ_OBJ_DETECT:
			break;
			case RQ_REP_OBJ_DETECT:
				{	
					cout<<"Request for face detection"<<endl;
					/*if(NULL == bd_p)
					{
						bd_p= new BallDetector;
					}*/
					/*if(NULL != bd_p)*/ bd.startRepeatedDetect(socket);
				}
				break;

			case CNCL_REP_OBJ_DETECT:
				{	
					//delete bd_p;
					//bd_p = NULL;
					bd.stopRepeatDetect();
				}
				break;

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
