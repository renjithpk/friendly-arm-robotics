/*
 * wheel.h
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#ifndef WHEEL_H_
#define WHEEL_H_


#include <time.h>
#include "ioport.h"
#include "thread.h"


class wheel :  public Thread
{

	unsigned int	onTime; 	//unit millisecond
	unsigned int	offTime;		//millisecond
	unsigned int	timePeriod;
	uint8_t 	dutyCycle; 	// in %
	bool		isForward; // true forward / false reverse
	I2CPort*	port;
	uint8_t		pol1;
	uint8_t		pol2;
	char        type;
	virtual void Execute(void*);
public:

	wheel(char aType, unsigned int aPeriod = 100000);
	void setSpeed(uint8_t speed);
	void setDirection(bool aIsforward);
	void turnOn();
	void turnOff();

};

#endif /* WHEEL_H_ */
