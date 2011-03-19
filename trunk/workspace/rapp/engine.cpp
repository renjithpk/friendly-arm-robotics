/*
 * engine.cpp
 *
 *  Created on: Mar 17, 2011
 *      Author: renjith
 */
#include "types.h"
#include "engine.h"

engine::engine() {
	// TODO Auto-generated constructor stub
	pMotorR = new wheel('R');
	pMotorL = new wheel('L');
	speed	= 0;
	isForward = true;

}

engine::~engine() {
	// TODO Auto-generated destructor stub
	delete pMotorL;
	delete pMotorR;
}


void engine::Start(){
	pMotorL->Start(NULL);
	pMotorR->Start(NULL);
}


void engine::SetSpeed(uint8_t aSpeed){
	speed = aSpeed;
	pMotorL->setSpeed(aSpeed);
	pMotorR->setSpeed(aSpeed);

}


//true is forward and false is reverse
void engine::SetDirection(bool aIsForward){
	isForward = aIsForward;
	pMotorL->setDirection(aIsForward);
	pMotorR->setDirection(aIsForward);
}

// range 0 - 200, speed will vary from 100 to 200 reverse direction
void engine::MoveRight(uint8_t aAngle){
	uint16_t newSpeed = 0;

	if(aAngle > 200) return;

	if(aAngle > 100 ) {
            pMotorR->setDirection(!isForward);
            aAngle -= 100;
    }
    aAngle -= 100;

    if(aAngle != 0){
    	 newSpeed = (speed * aAngle)/100;
    }

    pMotorL->setSpeed(newSpeed);
    pMotorR->setSpeed(speed);
}

void engine::MoveLeft(uint8_t aAngle){
	uint16_t newSpeed = 0;

	if(aAngle > 200) return;

	if(aAngle > 100 ) {
            pMotorL->setDirection(!isForward);
            aAngle -= 100;
    }
    aAngle = 100 - aAngle;

    if(aAngle != 0){
    	 newSpeed = (speed * aAngle)/100;
    }

    pMotorR->setSpeed(newSpeed);
    pMotorL->setSpeed(speed);
}
