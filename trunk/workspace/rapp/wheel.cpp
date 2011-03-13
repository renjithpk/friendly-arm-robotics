/*
 * wheel.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"
wheel::wheel(uint16_t aPeriod )
{
	DBG_PRINT("wheelConstructor");
	timePeriod 	= aPeriod; //1 second;
	isForward 	= true;
	onTime = timePeriod;
	offTime = 0;
	port = I2CPort::Create();
	cout<<port<<"\n";
}

void wheel::setSpeed(uint8_t aSpeed)
{
	onTime = (timePeriod*aSpeed)/100;
	offTime = timePeriod - onTime;
	cout<<" on :"<<onTime<<" off "<<offTime<<"\n";
}
void wheel::setDirection(bool aIsForward)
{
	isForward = aIsForward;
}

void wheel::run()
{
	timespec tsR,tsE;
	DBG_PRINT("run motor");

	while(1){

		tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
		tsR.tv_nsec = onTime * 1000000; //millisecond to nano second
		nanosleep(&tsR,&tsE);
		turnOn();
		tsR.tv_sec 	= 0;// offTime/1000; //onTime is in millisecond
		tsR.tv_nsec = offTime * 1000000; //millisecond to nano second
		nanosleep(&tsR,&tsE);
		turnOff();
	}
}

inline void wheel::turnOn()
{
	//I2CPort port = I2CPort::Create();
	if(isForward){
		DBG_PRINT("forward  ON ");
		port->setpin(1,0);
		port->setpin(0,1);
	}
	else{
		port->setpin(0,0);
		port->setpin(1,1);
	}
}

inline void wheel::turnOff()
{
	//I2CPort port = I2CPort::Create();
	DBG_PRINT("forward  OFF ");
	port->setpin(0,0);
	//port->setpin(0,1);
}
