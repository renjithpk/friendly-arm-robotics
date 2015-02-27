/*
 * engine.cpp
 *
 *  Created on: Mar 17, 2011
 *      Author: renjith
 */
#include "types.h"
#include "engine.h"

Engine * Engine::my_ptr = NULL;

Engine::Engine()
{
	// TODO Auto-generated constructor stub
	pMotorR = new wheel('R');
	pMotorL = new wheel('L');
	speed	= 0;
	isForward = true;

}

Engine *Engine::getInstance()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(my_ptr == NULL)
	{
		my_ptr = new Engine;
		my_ptr->SetSpeed(0);
		my_ptr->SetDirection(true);
		my_ptr->Start();
	}
	return my_ptr;
}

Engine::~Engine()
{
	// TODO Auto-generated destructor stub
	delete pMotorL;
	delete pMotorR;
}


void Engine::Start()
{
	pMotorL->Start(NULL);
	pMotorR->Start(NULL);
}


void Engine::SetSpeed(uint8_t aSpeed)
{
	if(aSpeed >100 )
	{
		syslog(LOG_ERR,"speed out of range [%d]",aSpeed);
		speed = 100;
	}
	else if( aSpeed <0)
	{
		syslog(LOG_ERR,"speed out of range [%d]",aSpeed);
	speed = 0;
	}
	else
	{
		speed = aSpeed;
	}
	pMotorL->setSpeed(speed);
	pMotorR->setSpeed(speed);
}

//true is forward and false is reverse
void Engine::SetDirection(bool aIsForward)
{
	isForward = aIsForward;
	pMotorL->setDirection(aIsForward);
	pMotorR->setDirection(aIsForward);
}

// range 0 - 200, speed will vary from 100 to 200 reverse direction
void Engine::MoveRight(uint8_t aAngle)
{
	uint16_t newSpeed = 0;
	if(aAngle > 200) return;
	SetDirection(true);
	if(aAngle > 100 ) 
    {
		pMotorR->setDirection(false);
		//printf("MLLL -RRR ");
		aAngle -= 100;
	}
	else
    {
		aAngle = 100 -	aAngle;
	}
	if(aAngle != 0)
    {
		newSpeed = (speed * aAngle)/100;

		pMotorR->setSpeed(newSpeed);
		pMotorL->setSpeed(speed);
		//printf(" ML = %d MR = %d\n",(int)newSpeed,(int)speed);
	}
	else
    {
		//new
		pMotorR->setSpeed(0);
		pMotorL->setSpeed(speed);
	}
}

void Engine::MoveLeft(uint8_t aAngle)
{
	uint16_t newSpeed = 0;

	if(aAngle > 200)
	{
		syslog(LOG_ERR,"invalid range [%d]",aAngle);
		return;
	}

	SetDirection(true);
	if(aAngle > 100 )
    {
		pMotorL->setDirection(false);
		//printf("MLLL -RRR ");
		aAngle -= 100;
	}
	else
    {
		aAngle = 100 -	aAngle;
	}
	if(aAngle != 0)
    {
		newSpeed = (speed * aAngle)/100;

		pMotorL->setSpeed(newSpeed);
		pMotorR->setSpeed(speed);
		//printf(" ML = %d MR = %d\n",(int)newSpeed,(int)speed);
	}
	else
    {
		//new
		pMotorL->setSpeed(0);
		pMotorR->setSpeed(speed);
		//printf(" ML = %d MR = %d ---a=0\n",0,(int)speed);

	}

}

void Engine::SpeedUp()
{
	if(isForward == true)
	{
		syslog(LOG_INFO,"increase forward speed");
		SetSpeed(speed +10);
	}
	else
	{
		if((speed - 10) < 0)
		{
			syslog(LOG_INFO,"change to forward");
		    SetDirection(true);
			SetSpeed(10 - speed);
		}
		else 
		{
			syslog(LOG_INFO,"Decrease reverse speed");
			SetSpeed(speed - 10);
		}
	}
}

void Engine::SpeedDown()
{
	if(isForward == false)
	{
		syslog(LOG_INFO,"increase reverse speed");
		SetSpeed(speed +10);
	}
	else
	{
		if((speed - 10) < 0)
		{
			syslog(LOG_INFO,"change to reverse");
		    SetDirection(false);
			SetSpeed(10 - speed);
		}
		else 
		{
			syslog(LOG_INFO,"decrease forward speed");
			SetSpeed(speed -10);
		}
	}
}
