/*
 * testApp.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: renjith
 */

#include "wheel.h"

int main()
{
	motor * mptr = new wheel(2000);
	mptr->setSpeed(50);
	((wheel *)mptr)->run();

	delete mptr;
}
