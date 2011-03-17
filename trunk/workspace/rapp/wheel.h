/*
 * wheel.h
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include <time.h>
#include "ioport.h"
#include "Thread.h"
#ifndef WHEEL_H_
#define WHEEL_H_


/*class motor
{
public:
	virtual void setSpeed(uint8_t speed) = 0;
	virtual void setDirection(bool aIsforward) = 0;
	virtual
};*/

class wheel :  public Thread
{

	uint16_t	onTime; 	//unit millisecond
	uint16_t	offTime;		//millisecond
	uint16_t	timePeriod;
	uint8_t 	dutyCycle; 	// in %
	bool		isForward; // true forward / false reverse
	I2CPort*	port;
	uint8_t		pol1;
	uint8_t		pol2;

	virtual void Execute(void*);
public:

	wheel(char aType, uint16_t aPeriod = 35535);
	//void run();
	void setSpeed(uint8_t speed);
	void setDirection(bool aIsforward);
	void turnOn();
	void turnOff();

};

#endif /* WHEEL_H_ */