/*
 * wheel.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"
wheel::wheel(uint16_t aPeriod )
{
	timePeriod 	= aPeriod; //1 second;
	isForward 	= true;
	onTime = timePeriod;
	offTime = 0;
}

void wheel::setSpeed(uint8_t aSpeed)
{
	onTime = (aSpeed*100)/timePeriod;
	offTime = timePeriod - onTime;
}
void wheel::setDirection(bool aIsForward)
{
	isForward = aIsForward;
}

void wheel::run()
{
	timespec tsR,tsE;

	while(1)
	{
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
	I2CPort port = I2CPort::Create();
	if(isForward){
		port.setpin(1,0);
		port.setpin(0,1);
	}
	else{
		port.setpin(0,0);
		port.setpin(1,1);
	}
}

inline void wheel::turnOff()
{
	I2CPort port = I2CPort::Create();
	port.setpin(0,0);
	port.setpin(0,1);
}
