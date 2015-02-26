#ifndef PERSON_DETECT_H_
#define PERSON_DETECT_H_


#include "tcpsocket.h"
#include "cvInterface.h"
#include "event.h"
#include "thread.h"

pthread_mutex_t cam_mtx = PTHREAD_MUTEX_INITIALIZER;
class PerDetect: public Thread
{
	bool isNewImage;
	Mat imgToSend;
	cvIf cvif;
	public:
		PerDetect():cvif(true)
		{
			isNewImage = false;
		}
		int captureImage()
		{
			while(1)
			{
				//cout<<"get image  start"<<time(0)<<endl;
				Mat *imgp = cvif.getImage();
				//cout<<"get image    end"<<time(0)<<endl;
				if(imgp == NULL) { cout<< "couldnt capture"<<endl; continue; }
				if(EBUSY != pthread_mutex_trylock(&cam_mtx))
				{
					//cout<<"copy image start"<<time(0)<<endl;
					imgp->copyTo(imgToSend);
					//cout<<"copy image   end"<<time(0)<<endl;
					pthread_mutex_unlock(&cam_mtx);
					isNewImage = true ;
				}
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
			header.type = HEADER_IMAGE;
			header.size = cvif.getImgSize();

			while(1)
			{

//				socket.send((unsigned char *)&header,sizeof(Header));

				while(!isNewImage)
				{
					sleep(1);
				}
				/////send image
				pthread_mutex_lock(&cam_mtx);
//				cout<<"sendL start"<<time(0)<<endl;
//				if(-1 == socket.sendL((unsigned char *)imgToSend.data,header.size) )
//				{
//					return -1;
//				}
				vector< Rect_<int> > faces;
				cvif.detectFace(imgToSend,faces);
				for(int i = 0; i < faces.size(); i++)
				{
					cout<<"++++++ \n";//<<faces[i].area<<endl;
				}
				isNewImage = false;
//				cout<<"SendL   end"<<time(0)<<endl;
				pthread_mutex_unlock(&cam_mtx);
				/////send image done 
/*				
				
				Header headerS;
				if (socket.recv((unsigned char *)&headerS, sizeof(Header)) == -1) {
					cout<<"failed to receive data [client error]"<<endl;
					return 0;
				}
				if(headerS.type == HEADER_RESULT)
				{}

*/
			}
		}

};

#endif// PERSON_DETECT_H_
