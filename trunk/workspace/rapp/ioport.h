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

class I2CPort
{
private:
	char add;
	char data;
	int fd;
public:
	I2CPort(char aAdd = 0x20);
	static I2CPort& Create(void); //this will act as a singleton object
	
	void write(char aData);
	char read();
};




#endif//_ioport_h
