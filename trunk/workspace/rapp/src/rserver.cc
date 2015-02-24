#include "types.h"
#include "Thread.h"
#include "my_socket.h"
#include "cvInterface.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
bool isObjDetected = false;
//Mat streamImg;
pthread_mutex_t img_mtx;
Mat img;


pthread_mutex_t circle_mtx;
RCircle rCircle;//x; y;r; maxx; maxy;count;

void copyRCircle(RCircle & c,Vec3f circleVec)
{
			c.x = cvRound(circleVec[0]);
			c.y = cvRound(circleVec[1]);
			c.r = cvRound(circleVec[2]);
}

class CaptureImage:public Thread
{


	public:
		//	cvIf cv;
		//	CaptureImage():cv("192.168.0.102") {}

		void Execute(void*)
		{

			pthread_mutex_lock(&img_mtx);
			string streamAdd = "http://";
			streamAdd = streamAdd + "192.168.0.100"/*ip*/;
			streamAdd = streamAdd + ":8080/?action=stream&amp;type=.mjpg";
			VideoCapture vcap(streamAdd);
			while(!vcap.isOpened())
			{
				cout<<"failed to open stream \n retry...."<<endl;
				sleep(5);
			}
			vcap>> img;
			pthread_mutex_unlock(&img_mtx);
			while(1)
			{

				pthread_mutex_lock(&img_mtx);
				vcap>> img;
				pthread_mutex_unlock(&img_mtx);
				waitKey(10);
			}
		}
};
/*
void incrimentCount(void )
{
	if(rCircle.count <10)
	{
		rCircle.count++;
	}
}*/



//BALL DETECTOR
//BALL DETECTOR
//BALL DETECTOR


class BallDetector:public Thread
{
	public:
	cvIf cv;

	void Execute(void*)
	{
		cv.createBallTracker();
		waitKey(50);

		while(1)
		{
			int count = detectBall();
		}
	}
	int detectBall()
	{
		int ret = 0;
		int countBall = 0,tries = 0;
		vector<Vec3f> circles;
		Mat copiedImg;
		while(countBall == 0)	
		{
			if(tries++ > 5)
			{
				//pthread_mutex_lock(&circle_mtx);
				//rCircle.count = 0;
				//pthread_mutex_unlock(&circle_mtx);
				isObjDetected = false;
				return 0;
			}
			pthread_mutex_lock(&img_mtx);
			copiedImg = img.clone();
			pthread_mutex_unlock(&img_mtx);
			countBall = cv.detectBall(circles,copiedImg);
			if(countBall > 0)
			{
				cv.drawCircle(copiedImg,circles);
			}
			else
			{
				cv.displayImgage(copiedImg);
			}
			waitKey(10);
		}
		if(countBall == 1)
		{
			pthread_mutex_lock(&circle_mtx);
			isObjDetected = true;
			copyRCircle(rCircle, circles[0]);
			pthread_mutex_unlock(&circle_mtx);
		}
		else cout<<"multiple object found tune detector"<<endl;
		return countBall;
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
			//Header head_fd;
			//head_fd.type = DTD_OBJ_FACE;

			MSG_Circle msg;
			if(isObjDetected)
			{
				msg.header.type = DTD_OBJ_BALL;
				pthread_mutex_lock(&circle_mtx);
					//RCircle temp = rCircle;
					msg.rCircle = rCircle;
				pthread_mutex_unlock(&circle_mtx);
				socket.send((unsigned char *)&msg,sizeof(MSG_Circle));
			}
			else
			{
				msg.header.type = DTD_OBJ_NOBALL;
				socket.send((unsigned char *)&msg,sizeof(MSG_Circle));

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
			cv.displayImgage(img,rect);
		
		}else
		{
			cv.displayImgage(img);
		}

		waitKey(50);	
		return ret;
	}
#endif

};


int main(int argc,char * argv[])
{
	MyApp myApp;
	
	if(argc == 2)
	{
		string cip = argv[1];
	}

	myApp.startme();
}
