#ifndef RTIMER_H_
#define RTIMER_H_


#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <epoll.h>

	
class RTimer: public EpollIf 
{
	int fd;
public:
	int setDelay(const unsigned long &delayms);

	int setDelayRep(const unsigned long &delayms);
	virtual void onTimerExpired() = 0;

private:
	int initialize(struct itimerspec &new_value);
	void epoll_cb();
public:
	~RTimer();
};

#endif
