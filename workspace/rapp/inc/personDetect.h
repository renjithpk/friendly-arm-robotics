#ifndef PERSON_DETECT_H_
#define PERSON_DETECT_H_


#include "my_socket.h"
#include "cvInterface.h"
#include "Event.h"
#include "Thread.h"
//TODO move
struct Header
{
	int type;
	int size;
};
pthread_mutex_t cam_mtx;
class PerDetect: public Thread
{
	bool readyToSend;
	Mat imgToSend;
	cvIf cvif;
	public:
		PerDetect():cvif(true)
		{
			readyToSend = false;
		}
		int captureImage()
		{
			while(1)
			{
				Mat *imgp = cvif.getImage();
				if(imgp == NULL) { cout<< "couldnt capture"<<endl; continue; }
				if(EBUSY != pthread_mutex_trylock(&cam_mtx))
				{	
					imgp->copyTo(imgToSend);
					pthread_mutex_unlock(&cam_mtx);
				}
				readyToSend = true;
				sleep(1);
			}
		}
		virtual void Execute(void*)
		{
			captureImage();
		}
		int initialize()
		{
			Start(NULL);
			Socket socket;
			if( -1 == socket.initClient("192.168.2.103", 5000 ))
			{
				cout<<"init failed"<<endl;
				return -1;
			}	
			char buffer[600+1];
			Header header;
			header.type = 12345;
			header.size = cvif.getImgSize();
			while(1)
			{
				socket.send((unsigned char *)&header,sizeof(Header));
				while(!readyToSend) sleep(1);
				pthread_mutex_lock(&cam_mtx);
				if(-1 == socket.sendL((unsigned char *)imgToSend.data,header.size) ) return -1;
				pthread_mutex_unlock(&cam_mtx);
				readyToSend = false;
			}
		}

};

#endif// PERSON_DETECT_H_
