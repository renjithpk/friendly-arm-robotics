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
	//pMotorL = new wheel('L');

}

engine::~engine() {
	// TODO Auto-generated destructor stub
	delete pMotorR;
}
void engine::Start(){
	pMotorR->Start(NULL);
}
void engine::SetSpeed(uint8_t aSpeed){
	pMotorR->setSpeed(aSpeed);

}
//true is forward and false is reverse
void engine::SetDirection(bool aIsForward){

}
// range 0 - 200, speed will vary from 100 to 200 reverse direction
void engine::MoveRight(uint8_t aAngle){

}

void engine::MoveLeft(uint8_t aAngle){

}
