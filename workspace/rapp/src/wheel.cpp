/*
 * wheel.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"
wheel::wheel(char aType, uint16_t aPeriod)
{
	syslog(LOG_INFO,"wheelConstructor");
	timePeriod 	= aPeriod; //1 second;
	isForward 	= true;
	onTime 		= 0;
	offTime 	= timePeriod;
	port 		= I2CPort::Create();
	if('L' == aType)
	{
		pol1  = 0;
		pol2  = 1;
	}
	else if('R' == aType)
	{
		pol1  = 2;
		pol2  = 3;
	}
	type = aType;
}

void wheel::setSpeed(uint8_t aSpeed)
{

	//if(0 == aSpeed || aSpeed > 100) return; //:TODO 0 => all off 100 => full speed should not be any -ve values


	if(0 == aSpeed)
	{
		onTime = 0;
		offTime = timePeriod;
	}
	else if(100 <= aSpeed)
	{
		onTime = timePeriod;
		offTime = 0;
	}
	onTime = (timePeriod*aSpeed)/100;
	offTime = timePeriod - onTime;
	
	//syslog(LOG_INFO,"set speed %d %c ON %d OFF %d",aSpeed,type,onTime,offTime);
}
void wheel::setDirection(bool aIsForward)
{
	isForward = aIsForward;
}

void wheel::Execute(void*)
{
	timespec tsR,tsE;
	syslog(LOG_INFO,"run motor");

	while(1)
	{

		if(0 != onTime)
		{
			turnOn();
			//syslog(LOG_DEBUG,"on %c",type);
			tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
			tsR.tv_nsec = onTime * 1000; //microsecond to nano second
			nanosleep(&tsR,&tsE);
		}
		if(0 != offTime)
		{
			turnOff();
			//syslog(LOG_DEBUG,"off %c",type);
			tsR.tv_sec 	= 0;// offTime/1000; //onTime is in millisecond
			tsR.tv_nsec = offTime * 1000; //microsecond to nano second
			nanosleep(&tsR,&tsE);
		}

	}

}


inline void wheel::turnOn()
{

	uint8_t data = port->readPort();
	if(isForward)
	{
		data = data | 1 << pol1;
		data =  data & ~(1 << pol2); 
	}
	else
	{
		data = data | 1 << pol2;
		data =  data & ~(1 << pol1); 
	}
	port->setPort(data);
}

inline void wheel::turnOff()
{

	uint8_t data = port->readPort();
	data =  data & ~(1 << pol1); 
	data =  data & ~(1 << pol2); 
	port->setPort(data);
}
