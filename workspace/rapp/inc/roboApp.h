/*
 * roboApp.h
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#ifndef ROBOAPP_H_
#define ROBOAPP_H_

//#include "Event.h"
#include "appFW.h"
#include "engine.h"

class RoboApp: public AppIf
{
	Engine* engine;
	int 	speed;
	bool	isBlocked;
public:
	RoboApp();
	~RoboApp();
	int init(int argc,char * argv[]);
	int onNewObject(EMessageT oType,void * data);
	int onKeyPressed(int ch); 
};

#endif /* ROBOAPP_H_ */
	//bool EventCallBack(ESensor aType, bool state);