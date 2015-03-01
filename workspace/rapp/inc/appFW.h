#ifndef APPFW_H_
#define APPFW_H_
#include "epoll.h"
#include "tcpsocket.h"
#include "types.h"
#include "terminal.h"
#include "thread.h"

class KeyIf
{
	public:
		virtual void onKeyPressed(char key) = 0;
};

class Key :public EpollIf, public Thread
{
	int getKeyFd[2];
	KeyIf *m_keyif;
	public:
	void init(KeyIf *keyIf)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		pipe(getKeyFd);

		m_keyif = keyIf;

		if (pipe(getKeyFd) != -1)
		{
			initialize(getKeyFd[0]);
			Epoll::getInstance()->addFd(this);
			Start();

		}
		else
		{
			syslog(LOG_ERR,"ERROR: Key::init pipe() failed");
		}
	}
	void epoll_cb()
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		char key;
		read(getKeyFd[0],&key,1);
		m_keyif->onKeyPressed(key);
	}



	void Execute(void *arg)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		Terminal term;
		while(1)
		{
			char ch = term.getch();
			//char ch;
			//cin>>ch;
			syslog(LOG_DEBUG,"new key pressed :%d ",ch);
			write(getKeyFd[1],&ch,1);
		}
	}

	~Key()
	{
		if(NULL != m_keyif)	syslog(LOG_ERR,"Destructor Key, is it abnormal!!!");	
	}

};

class AppIf
{

	public:

		virtual int init(int argc,char *argv[]) {};
		virtual int onNewObject(EMessageT oType,void * data) {}
		virtual int onKeyPressed(int ch){} 
		virtual ~AppIf(){}
};


class App :public SocketIf, public KeyIf 
{
	Socket socket;
	Key key;
	AppIf * appIf;
	int argc;
	char **argv;
	string serverIp;
	public:
	App()
	{}

	App(int argc, char * argv[]):argc(argc),argv(argv)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);

		if(argc == 2)
		{
			serverIp = 	argv[1];
		}
		else
		{
			syslog(LOG_ERR,"missing server ip argument");
		}
	}
	int requestObj()
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		Header headern;
		headern.type = RQ_OBJ_DETECT;
		socket.send((unsigned char *)&headern,sizeof(Header));
	}
	int initService(void)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		socket.registerInterface(this);

		if( -1 == socket.initClient(serverIp, 5000 ))
		{
			syslog(LOG_ERR,"init failed");
			return -1;
		}			

		//requestObj();
	}

	int onDataReceived(unsigned char * buffer, int size)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		Header *header = (Header*) buffer;
		syslog(LOG_DEBUG,"on data received");
		if(NULL == header)
		{
			syslog(LOG_ERR,"NULL pointer");
			return -1;
		}
		switch (header->type)
		{
			case DTD_OBJ_FACE:
				{
					if(appIf != NULL)
					{
						appIf->onNewObject(header->type,buffer+sizeof(Header));
					}
					else
					{
						syslog(LOG_ERR,"NULL pointer");
					}
				}
				break;
			case DTD_OBJ_NOBALL:
			case DTD_OBJ_BALL:
			case DTD_OBJ_M_BALL:
				{
					if(appIf != NULL)
					{
						appIf->onNewObject(header->type,buffer+sizeof(Header));
					}
					else
					{
						syslog(LOG_ERR,"NULL pointer");
					}
				}
				break;

			default:
				{
					syslog(LOG_ERR,"unknown header");

				}break;



				//	MSG_Circle msg;
				//	msg.header.type = DTD_OBJ_BALL;
				//	msg.rCircle = temp;
		}
	}
	void Register(AppIf * appif)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		this->appIf=appif;
		appIf->init(argc,argv);
		initGetKey();
		initService();
		while(1)
		{
			syslog(LOG_INFO,"App Run {epoll->run()}");
			Epoll::getInstance()->run();
		}
		syslog(LOG_ERR,"this is imposible");
	}
	int initGetKey()
	{	
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		key.init(this);
	}
	void onKeyPressed(char key)
	{
		syslog(LOG_DEBUG, "%s ENTRY",__func__);
		appIf->onKeyPressed(key);
	}
	//void enableObjectDetect(); TODO
};

#endif
