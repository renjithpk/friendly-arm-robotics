/*
 * engine.cpp
 *
 *  Created on: Mar 17, 2011
 *      Author: renjith
 */
#include "types.h"
#include "engine.h"

Engine::Engine() {
	// TODO Auto-generated constructor stub
	pMotorR = new wheel('R');
	pMotorL = new wheel('L');
	speed	= 0;
	isForward = true;

}

Engine::~Engine() {
	// TODO Auto-generated destructor stub
	delete pMotorL;
	delete pMotorR;
}


void Engine::Start(){
	pMotorL->Start(NULL);
	pMotorR->Start(NULL);
}


void Engine::SetSpeed(uint8_t aSpeed)
{
	if(aSpeed >100 )
	{
		PRINT_4("speed out of range [%d]",aSpeed);
		speed = 100;
	}
	else if( aSpeed <0)
	{
		PRINT_4("speed out of range [%d]",aSpeed);
	speed = 0;
	}
	else
	{
		speed = aSpeed;
	}
	pMotorL->setSpeed(speed);
	pMotorR->setSpeed(speed);
}


void Engine::SpeedUp()
{
	if(isForward == true)
	{
		PRINT_1("increase forward speed");
		SetSpeed(speed +10);
	}
	else
	{
		if((speed - 10) < 0)
		{
			PRINT_1("change to forward");
		    SetDirection(true);
			SetSpeed(10 - speed);
		}
		else 
		{
			PRINT_1("Decrease reverse speed");
			SetSpeed(speed - 10);
		}
	}
}

void Engine::SpeedDown()
{
	if(isForward == false)
	{
		PRINT_1("increase reverse speed");
		SetSpeed(speed +10);
	}
	else
	{
		if((speed - 10) < 0)
		{
			PRINT_1("change to reverse");
		    SetDirection(false);
			SetSpeed(10 - speed);
		}
		else 
		{
			PRINT_1("decrease forward speed");
			SetSpeed(speed -10);
		}
	}
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
		PRINT_4("invalid range [%d]",aAngle);
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
