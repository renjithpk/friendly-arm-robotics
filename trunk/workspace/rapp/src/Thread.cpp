/*
 * Thread.cpp
 *
 *  Created on: Mar 17, 2011
 *      Author: renjith
 */

#include "Thread.h"

Thread::Thread() {
	// TODO Auto-generated constructor stub

}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
}
int Thread::Start(void * aArg){
	arg = aArg;
	return pthread_create( &thread, NULL, Thread::EntryPoint, (void*)this );
}

void * Thread::EntryPoint(void* aArg){
	((Thread*)aArg)->Execute(NULL);
	return NULL;
}
