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

bool RoboApp::EventCallBack(ESensor aType, bool state)
{
	cout << int(aType)<<" state " << (int)state<<" \n ";
}








