/*
 * testApp.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"
#include <pthread.h>
#include <iostream>
using namespace std;

void* startEgine(void * pMotor)
{
	if(pMotor == NULL ) throw 1;
	wheel *pWheel = static_cast<wheel*>(pMotor);
	pWheel->run();
	return NULL;
}

int main()
{
	pthread_t thread_engine;
	DBG_PRINT("start APP");
	motor * mptr = new wheel(10);
	DBG_PRINT("set speed");
	mptr->setSpeed(1);
	pthread_create( &thread_engine, NULL, startEgine, (void*)mptr );

	uint8_t speed = 0;
	sleep(3);
	speed +=2;
	cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
	mptr->setSpeed(speed);
	sleep(3);
	speed +=10;
	cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
	mptr->setSpeed(speed);
	while(1){
		sleep(3);
		speed +=10;
		cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
		mptr->setSpeed(speed);

	}
	//DBG_PRINT("Run motor");
	//((wheel *)mptr)->run();

}
