/*
 * testApp.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"

int main()
{
	DBG_PRINT("start APP");
	motor * mptr = new wheel;
	DBG_PRINT("set speed");
	mptr->setSpeed(50);
	DBG_PRINT("Run motor");
	((wheel *)mptr)->run();

}
