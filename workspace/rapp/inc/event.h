/*
 * SensorEvent.h
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#ifndef SENSOREVENT_H_
#define SENSOREVENT_H_

#include "ioport.h"
#include "types.h"

enum ESensor{
	ESensor_IR = 4,
	ESensor_PIR
};

class Event {
	I2CPort * 	port;
	uint8_t 	data;
public:
	Event();
	int RunApp(void);
	virtual ~Event();
	virtual bool EventCallBack(ESensor aType, bool state);
	//virtual bool FaceDetected(ESensor aType, bool state);
};

#endif /* SENSOREVENT_H_ */
