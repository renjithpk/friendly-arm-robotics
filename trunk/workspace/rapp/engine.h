/*
 * engine.h
 *
 *  Created on: Mar 17, 2011
 *      Author: renjith
 */

#ifndef ENGINE_H_
#define ENGINE_H_
#include "wheel.h"

class engine {
	wheel*		pMotorL; // = new wheel(TIME_PERIOD);
	wheel*		pMotorR; // = new wheel(TIME_PERIOD);
	uint16_t	speed;
	bool		isForward;
public:
	engine();
	virtual ~engine();
	//set speed for both right and left wheel, and speed will be same
	void SetSpeed(uint8_t aSpeed);
	//true is forward and false is reverse
	void SetDirection(bool aIsForward);
	// range 0 - 200, speed will vary from 100 to 200 reverse direction
	void MoveRight(uint8_t aAngle);
	void MoveLeft(uint8_t aAngle);
	void Start();
};

#endif /* ENGINE_H_ */
