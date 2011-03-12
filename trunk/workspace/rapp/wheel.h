/*
 * wheel.h
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include <time.h>
#include "ioport.h"
#ifndef WHEEL_H_
#define WHEEL_H_

class motor
{
public:
	virtual void setSpeed(uint8_t speed) = 0;
	virtual void setDirection(bool aIsforward) = 0;
};

class wheel : public motor
{

	uint16_t	onTime; 	//unit millisecond
	uint16_t	offTime;		//millisecond
	uint16_t	timePeriod;
	uint8_t 	dutyCycle; 	// in %
	bool		isForward; // true forward / false reverse
//	I2CPort		port;

public:

	wheel(uint16_t aPeriod = 1000);
	void run();
	void setSpeed(uint8_t speed);
	void setDirection(bool aIsforward);
	void turnOn();
	void turnOff();
};

#endif /* WHEEL_H_ */
