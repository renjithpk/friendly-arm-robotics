/*
 * RoboApp.h
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#ifndef ROBOAPP_H_
#define ROBOAPP_H_

#include "Event.h"
#include "engine.h"

class RoboApp: public Event {
	Engine* engine;
	int 	speed;
public:
	RoboApp();
	virtual ~RoboApp();
	int Initialize(void);
	bool EventCallBack(ESensor aType, bool state);
};

#endif /* ROBOAPP_H_ */
