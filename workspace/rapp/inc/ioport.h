#ifndef _ioport_h
#define _ioport_h

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "types.h"



class I2CPort
{
private:
	static I2CPort *myPtr;
	static uint8_t count;
	int fd;
	int8_t add;
	uint8_t data;
	I2CPort(uint8_t aAdd = 0x20);

public:

	static I2CPort* Create(void); //this will act as a singleton object
	static void Release(void); //this will act as a singleton object
	
	void setPort(uint8_t aData);
	uint8_t readPort();
	void setpin(bool state,uint8_t pinNumber);
	bool getPin(uint8_t pinNumber);

	void display();

	~I2CPort();
};



#endif//_ioport_h
