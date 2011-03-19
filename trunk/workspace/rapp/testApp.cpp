/*
 * testApp.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */


#include "engine.h"
#include <iostream>
using namespace std;

/*void* startEgine(void * pMotor)
{
	if(pMotor == NULL ) throw 1;
	wheel *pWheel = static_cast<wheel*>(pMotor);
	pWheel->run();
	return NULL;
}*/

int main()
{
//	pthread_t thread_engine;
	DBG_PRINT("start APP");
/*	motor * mptr = new wheel(10000);
	DBG_PRINT("set speed");
	mptr->setSpeed(1);
	pthread_create( &thread_engine, NULL, startEgine, (void*)mptr );*/

	uint8_t speed = 0;

	engine* pEngine = new engine();



	speed =60;
	cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
	pEngine->SetSpeed(speed);
	pEngine->Start();
#if 0
	sleep(3);
	pEngine->SetDirection(false);
	sleep(3);
	pEngine->SetDirection(true);
	sleep(3);
	pEngine->MoveLeft(50);
	sleep(3);
	pEngine->MoveLeft(150);
	sleep(3);
	pEngine->MoveLeft(200);
	sleep(3);
	pEngine->MoveRight(50);
	sleep(3);
	pEngine->MoveRight(150);
	sleep(3);
	pEngine->MoveRight(200);
	sleep(3);
	pEngine->SetSpeed(0);

	return 0;
#endif
//
	sleep(3);
	speed =1;
	cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
	pEngine->SetSpeed(speed);
	pEngine->Start();
	sleep(3);
	speed =5;
	cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
	pEngine->SetSpeed(speed);
	sleep(3);
	speed =20;
	cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
	pEngine->SetSpeed(speed);

	while(150 > speed){
		sleep(3);
		speed +=20;
		cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
		pEngine->SetSpeed(speed);

	}
	//DBG_PRINT("Run motor");
	//((wheel *)mptr)->run();

}
