/*
 * wheel.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"
wheel::wheel(char aType, uint16_t aPeriod)
{
	PRINT_1("wheelConstructor");
	timePeriod 	= aPeriod; //1 second;
	isForward 	= true;
	onTime 		= timePeriod;
	offTime 	= 0;
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

	PRINT_1("set speed %d %c",aSpeed,type);

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
}
void wheel::setDirection(bool aIsForward)
{
	isForward = aIsForward;
}

void wheel::Execute(void*)
{
	timespec tsR,tsE;
	PRINT_1("run motor");

	while(1)
	{

		if(0 != onTime)
		{
			turnOn();
			tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
			tsR.tv_nsec = onTime * 1000; //microsecond to nano second
			nanosleep(&tsR,&tsE);
		}
		if(0 != offTime)
		{
			turnOff();
			tsR.tv_sec 	= 0;// offTime/1000; //onTime is in millisecond
			tsR.tv_nsec = offTime * 1000; //microsecond to nano second
			nanosleep(&tsR,&tsE);
		}

	}

}

/*void wheel::run()
{
	timespec tsR,tsE;
	PRINT_1("run motor");

	while(1){

		turnOn();
		tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
		tsR.tv_nsec = onTime * 1000; //microsecond to nano second
		nanosleep(&tsR,&tsE);
		turnOff();
		tsR.tv_sec 	= 0;// offTime/1000; //onTime is in millisecond
		tsR.tv_nsec = offTime * 1000; //microsecond to nano second
		nanosleep(&tsR,&tsE);

	}
}*/

inline void wheel::turnOn()
{
/*
	if(isForward){
		PRINT_1("forward  ON ");
		port->setpin(1,pol1);
		port->setpin(0,pol2);
	}
	else{
		port->setpin(0,pol1);
		port->setpin(1,pol2);
	}
	*/
	port->setpin(isForward,pol1); //to optimize the above code
	port->setpin(!isForward,pol2);

}

inline void wheel::turnOff()
{

	PRINT_2("forward  OFF ");
	isForward ? port->setpin(0,pol1): port->setpin(0,pol2);

}
