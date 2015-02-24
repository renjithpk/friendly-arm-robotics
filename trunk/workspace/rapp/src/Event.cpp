/*
 * SensorEvent.cpp
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#include "Event.h"
#include <unistd.h>

Event::Event() {
	// TODO Auto-generated constructor stub
	port = I2CPort::Create();
	data = 0;

}

int Event::RunApp(void)
{
	timespec tsR,tsE;
	tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
	tsR.tv_nsec = 10000000; //100 millisecond delay
	syslog(LOG_INFO,"inside Run App");
	while(1)
	{

		uint8_t cData = port->readPort();
		//trigger call back if current and previous data are different 
		if( (cData & ( 1 << ESensor_IR )) ^ (data & ( 1 << ESensor_IR )) ){
			syslog(LOG_DEBUG,"event triggered");
			this->EventCallBack( ESensor_IR ,cData & ( 1 << ESensor_IR ) );

		}
		data = cData;
		nanosleep(&tsR,&tsE);

	}
}

Event::~Event() {
	// TODO Auto-generated destructor stub
}


bool Event::EventCallBack(ESensor aType, bool state)
{
}




