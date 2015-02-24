// Definition of the Epoll class

#ifndef __Epoll_class_
#define __Epoll_class_

#include <sys/epoll.h>
#include <map>

#define MAX_EVENTS 10

using namespace std;



//derive class to be listned
//call initialize with fd
//Epoll::getInstance()->addFd(this);

class EpollIf
{
public:
	struct epoll_event ev;
	int fd_m; 
	EpollIf(const int &fd );
	EpollIf();
	void initialize(const int &fd );
	virtual void epoll_cb() = 0;
};

class Epoll
{
private:

	int epfd;
	struct epoll_event events[MAX_EVENTS];
	std::map <int, EpollIf*> intList;
	static Epoll *my_ptr;
	Epoll();
public:
	int addFd(EpollIf *intEpoll); //TODO rename 
	int run();
	static Epoll * getInstance();
};


#endif
