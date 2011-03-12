#ifndef _ioport_h
#define _ioport_h

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include "types.h"
class I2CPort
{
private:
	int8_t add;
	uint8_t data;
	int fd;
	I2CPort(uint8_t aAdd = 0x20);
public:

	static I2CPort& Create(void); //this will act as a singleton object
	
	void write(uint8_t aData);
	uint8_t read();
	void setpin(bool state,uint8_t pinNumber);

#ifdef DBG_ON
	void display();
#endif
	~I2CPort();
};




#endif//_ioport_h
