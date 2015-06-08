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
//#include "engine.h"
#include "stateMachine.h"

class RoboApp: public AppIf
{
	//Engine* engine;
	Context context;
	int 	speed;
	int mode;
//	bool	isBlocked;
public:
	RoboApp();
	~RoboApp();
	int init(int argc,char * argv[]);
	int onNewObject(EMessageT oType,void * data);
	int onKeyPressed(int ch);
	int menu_0(int ch);
	int menu_1(int ch);
	int menu_2(int ch);
	int menu_3(int ch);
	int menu_4(int ch);
};

#endif /* ROBOAPP_H_ */
	//bool EventCallBack(ESensor aType, bool state);
