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
	static I2CPort *myPtr;
	I2CPort(char aAdd);
public:
	static I2CPort* Create();
	
	void write(char aData);
	char read();
};



#endif//_ioport_h
