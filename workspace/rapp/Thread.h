/*
 * Thread.h
 *
 *  Created on: Mar 17, 2011
 *      Author: renjith
 */

#ifndef THREAD_H_
#define THREAD_H_
#include <pthread.h>

class Thread {
	pthread_t	thread;
    void *		arg;
public:
	Thread();
	int Start(void * aArg = NULL);
	virtual ~Thread();
protected:
	static void * EntryPoint(void* aArg);
	virtual void Execute(void*) = 0;
};

#endif /* THREAD_H_ */
