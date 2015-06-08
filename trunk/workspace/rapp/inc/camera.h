/*
 * Camera.h
 *
 *  Created on: 06-Jun-2015
 *      Author: renjith
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>
#include <fstream>
#include <rtimer.h>
class Camera:public RTimer
{
	int aVN;
	int aHN;
	int aVC;
	int aHC;
	int sV;
	int sH;

	static Camera * my_ptr;
	Camera();
	std::ofstream devfile;
public:
	virtual ~Camera();
	void startStream();
	void moveV(int angle, int speed = 0);
	void moveH(int angle, int speed = 0);
	bool isFileExist(const std::string& name);
	static Camera *getInstance();
	void onTimerExpired();
};

#endif /* CAMERA_H_ */
