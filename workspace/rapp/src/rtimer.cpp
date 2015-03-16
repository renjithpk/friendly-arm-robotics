#include <rtimer.h>


int RTimer::setDelay(const unsigned long &delayms)
{
	struct itimerspec new_value = {0};
	new_value.it_value.tv_sec = delayms/1000;
	new_value.it_value.tv_nsec = (delayms % 1000)* 1000000;
	return initialize(new_value);
}


int RTimer::setDelayRep(const unsigned long &delayms)
{
	struct itimerspec new_value = {0};
	new_value.it_value.tv_sec = delayms/1000;
	new_value.it_value.tv_nsec = (delayms % 1000)* 1000000;
	new_value.it_interval.tv_sec = delayms/1000;
	new_value.it_interval.tv_nsec = (delayms % 1000)* 1000000;
	return initialize(new_value);
}



int RTimer::initialize(struct itimerspec &new_value)
{

	/* Create a CLOCK_REALTIME absolute timer with initial
	   expiration and interval as specified in command line */

	fd = timerfd_create(CLOCK_REALTIME, 0);
	if (fd == -1)
	{
		//handle_error("timerfd_create");
		return -1;
	}

	if (timerfd_settime(fd, /*TFD_TIMER_ABSTIME*/ 0 , &new_value, NULL) == -1)
	{
		//	handle_error("timerfd_settime");
		return -1;
	}

	EpollIf::initialize(fd);
	Epoll::getInstance()->addFd(this);
	return 0;
}


void RTimer::epoll_cb()
{
	uint64_t exp;
	ssize_t s;
	s = read(fd, &exp, sizeof(uint64_t));
	if (s != sizeof(uint64_t))
	{
		//	handle_error("read");
	}
	onTimerExpired();
}


RTimer::~RTimer()
{
	::close(fd);	
}
