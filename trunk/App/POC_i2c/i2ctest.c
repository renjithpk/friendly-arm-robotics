#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/i2c-dev.h>


int main()
{
	int file;
	char buf[2];
	file = open("/dev/i2c/0",O_RDWR);

	if(file < 0){
		printf("error opening device \n");
		exit(1);
	}

	printf(" open device success \n");

	if( ioctl(file, I2C_SLAVE, 0x20) < 0){
		printf("error setting address \n");
		exit(1);
	}

	printf(" setting adress success \n");
	
	buf[0] = 0xFF;
	buf[1] = 0xff;

	write(file, buf, 1);
	printf("sent first byte \n");

	sleep(2);
	buf[0] = 0x0;
	buf[1] = 0x0;
	
	write(file, buf, 1);
	printf("sent second byte \n");

	sleep(2);
	buf[0] = 0xFF;
	buf[1] = 0xff;
	write(file, buf, 1);
	printf("sent first byte \n");

	sleep(2);
	buf[0] = 0x0;
	buf[1] = 0x0;
	write(file, buf, 1);
	printf("sent second byte \n");
	
	return 0;
}
