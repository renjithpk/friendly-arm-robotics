#ifndef APPFW_H_
#define APPFW_H_
#include "tcpsocket.h"
#include "types.h"
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
	void init(KeyIf *keyIf);
	void epoll_cb();
	void Execute(void *arg);
	~Key();
};

class AppIf
{
	public:
		virtual int init(int argc,char *argv[]);
		virtual int onNewObject(EMessageT oType,void * data);
		virtual int onKeyPressed(int ch);
		virtual ~AppIf();
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
	App();
	App(int argc, char * argv[]);
	int requestObj();
	int initService(void);
	int onDataReceived(unsigned char * buffer, int size);
	void Register(AppIf * appif);
	int initGetKey();
	void onKeyPressed(char key);
	void exit();
};

#endif
