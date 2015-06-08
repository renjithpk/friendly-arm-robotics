/*
 * camera.cpp
 *
 *  Created on: 06-Jun-2015
 *      Author: renjith
 */

#include <sys/types.h>
#include <sys/stat.h>
#include "camera.h"
#include <unistd.h>
#include "types.h"
#include "stdlib.h"
#include <fcntl.h>
#include <fstream>
using namespace std;


Camera * Camera::my_ptr  = NULL;

Camera::Camera()
{
	aVC = 0;
	aHC = 0;
	aVN = 0;
	aHN = 0;
	sH = 0;
	sV = 0;
}

Camera::~Camera()
{
	// TODO Auto-generated destructor stub
}

Camera *Camera::getInstance()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(my_ptr == NULL)
	{
		my_ptr = new Camera;
		if(!my_ptr->isFileExist("/dev/servoblaster"))
		{
			if(-1 != system("./servod"))
			{
				syslog(LOG_DEBUG, "loaded servod");
			}
			else
			{
				syslog(LOG_DEBUG, "failed to load");
			}
		}
  		my_ptr->devfile.open ("/dev/servoblaster");
	}
	return my_ptr;
}

bool Camera::isFileExist(const std::string& name)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
  	struct stat buffer;   
  	return (stat (name.c_str(), &buffer) == 0); 	
}
void Camera::moveH(int angle, int speed)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	//devfile<<"0="<<angle<<"%"<<endl;
	aVN = angle;
	sV = speed;
	onTimerExpired();
}



void Camera::moveV(int angle, int speed)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	//devfile<<"1="<<angle<<"%"<<endl;
	aHN = angle;
	sH = speed;
	onTimerExpired();
}


void Camera::onTimerExpired()
{
	bool isSetDelay = false;
	//if required angle less than current angle then reduce current angle

	if(aHN < aHC )
	{
		isSetDelay = true;
		aHC --;
		devfile<<"0="<<aHC<<"%"<<endl;
	}
	else if(aHC < aHN)
	{	
		isSetDelay = true;
		aHC ++;
		devfile<<"0="<<aHC<<"%"<<endl;
	}
	else if(aHC < aHN)
	{
		aHC = aHN;
		devfile<<"0="<<aHC<<"%"<<endl;	
	}
	//if required angle less than current angle then reduce current angle
	//  |----aHN------|<5>aHC------------------------------------------|

	if(aVN < aVC)
	{
		isSetDelay = true;
		aVC --;
		devfile<<"0="<<aVC<<"%"<<endl;
	}
	//  |-------------------aVC----<5>--|---------------aVN--------|
	else if(aVC < aVN)
	{	
		isSetDelay = true;
		aVC ++;
		devfile<<"0="<<aVC<<"%"<<endl;
	}
	if(isSetDelay)
	{
		setDelay(100);
	}
}
/*
void Camera::onTimerExpired()
{
	bool isSetDelay = false;
	//if required angle less than current angle then reduce current angle
	//  |----aHN------|<5>aHC------------------------------------------|

	if(aHN < (aHC - 5) )
	{
		isSetDelay = true;
		aHC -= 5;
		devfile<<"0="<<aHC<<"%"<<endl;
	}
	else if(aHN < aHC)
	{
		aHC = aHN;
		devfile<<"0="<<aHC<<"%"<<endl;
	}
	//  |-------------------aHC----<5>--|---------------aHN--------|
	else if((aHC + 5) < aHN)
	{	
		isSetDelay = true;
		aHC += 5;
		devfile<<"0="<<aHC<<"%"<<endl;
	}
	else if(aHC < aHN)
	{
		aHC = aHN;
		devfile<<"0="<<aHC<<"%"<<endl;	
	}
	//if required angle less than current angle then reduce current angle
	//  |----aHN------|<5>aHC------------------------------------------|

	if(aVN < (aVC - 5) )
	{
		isSetDelay = true;
		aVC -= 5;
		devfile<<"0="<<aVC<<"%"<<endl;
	}
	else if(aVN < aVC)
	{
		aVC = aVN;
		devfile<<"0="<<aVC<<"%"<<endl;
	}
	//  |-------------------aVC----<5>--|---------------aVN--------|
	else if((aVC + 5) < aVN)
	{	
		isSetDelay = true;
		aVC += 5;
		devfile<<"0="<<aVC<<"%"<<endl;
	}
	else if(aVC < aVN)
	{
		aVC = aVN;
		devfile<<"0="<<aVC<<"%"<<endl;	
	}
	if(isSetDelay)
	{
		setDelay(1);
	}
}

*/

void Camera::startStream()
{
}


