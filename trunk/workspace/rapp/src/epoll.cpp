// Definition of the Epoll class


#include "epoll.h"
#include <syslog.h>
#include <cstddef>


Epoll * Epoll::my_ptr = NULL;

/******     EpollIF *********/

EpollIf::EpollIf(const int &fd ):fd_m(fd)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
}

EpollIf::EpollIf()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	fd_m = -1;
}

void EpollIf::initialize(const int &fd )
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
	fd_m = fd;
}

/****** END    EpollIF *********/


/****** Epoll *********/

Epoll * Epoll::getInstance()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(my_ptr == NULL)
	{
		my_ptr = new Epoll;
	}
	return my_ptr;
}

Epoll::Epoll()
{
	epfd = epoll_create(5);
	syslog(LOG_INFO,"Epoll::Epoll() epfd : %d",epfd);
}

int Epoll::addFd(EpollIf *intEpoll)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	intList[intEpoll->fd_m] = intEpoll;
	int res = epoll_ctl(epfd, EPOLL_CTL_ADD,intEpoll->fd_m , &intEpoll->ev);
	syslog(LOG_DEBUG,"epoll fd added res %d epfd %d",res,epfd);
}

int Epoll::run()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	while(1)
	{
		syslog(LOG_DEBUG,"Epoll::run() before epollwait");
		int nfds = epoll_wait(epfd, events, 
				MAX_EVENTS, 
				-1);
		syslog(LOG_DEBUG,"Epoll::run() epoll unblocked");
		for (int i = 0; i < nfds; i++)
		{
			if ((events[i].events & EPOLLERR) ||
					(events[i].events & EPOLLHUP) ||
					(!(events[i].events & EPOLLIN)))
			{
				/* An error has occured on this fd, or the socket is not
				   ready for reading (why were we notified then?) */
				//fprintf (stderr, "epoll error\n");
				syslog(LOG_ERR,"Epoll::run() epoll wait error");
				//::close (events[i].data.fd);
				continue;
			}

			int tempfd = events[i].data.fd;
			if( intList.end() != intList.find(tempfd))
			{
				syslog(LOG_DEBUG,"Epoll::run() epoll calling epoll_cb");
				EpollIf * epollIf = intList[tempfd];
				if(NULL != epollIf)
				{
					epollIf->epoll_cb();
					break;
				}
				else
				{
					syslog(LOG_ERR,"Epoll::run() Error epollIf NULL, event ignored");
				}
			}
			else
			{
				syslog(LOG_ERR,"Epoll::run() fd not found in map");	
			}
		}
	}

}
