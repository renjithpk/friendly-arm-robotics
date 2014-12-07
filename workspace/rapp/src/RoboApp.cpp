/*
 * RoboApp.cpp
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#include "RoboApp.h"

int main()
{
	RoboApp* app = new RoboApp;
	app->Initialize();
}

RoboApp::RoboApp() {
	// TODO Auto-generated constructor stub
	engine 	= NULL; //instantiated inside initiazer
	speed 	= 0;
	isBlocked = false;
}

RoboApp::~RoboApp() {
	// TODO Auto-generated destructor stub
	delete engine;
}

int RoboApp::Initialize(void)
{
	engine = new Engine();
	engine->SetSpeed(speed);
	engine->Start();
	return RunApp();
}
//Ir sensor ESensor_IR, state - true/false true => blocked
bool RoboApp::EventCallBack(ESensor aType, bool state)
{
	//cout << int(aType)<<" state " << (int)state<<" \n ";
	if( aType == ESensor_IR && state == true  ){
		cout<<"LIR is ON "<<endl;
		engine->SetDirection(false);
		engine->SetSpeed(20);
	}
	else if( aType == ESensor_IR && state == false  ){
		sleep(1);
		engine->SetDirection(true);
		engine->MoveRight(200);
		sleep(1);
		engine->SetDirection(true);
		engine->SetSpeed(100);
	}

}








