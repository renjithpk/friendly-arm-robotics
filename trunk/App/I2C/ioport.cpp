#include "ioport.h"
#include "iostream"
#include "stdio.h"


using namespace std;


I2CPort::I2CPort(char aAdd = 0x20){
	fd = open("/dev/i2c/0",O_RDWR);

	if(fd < 0){
		cout<<"error opening device "<<endl;
		throw 1;
	}

	cout<<" open device success "<<endl;

	if( ioctl(fd, I2C_SLAVE, aAdd) < 0){
		cout<<"error setting address"<<endl;
		throw 1;
	}

}
void I2CPort::write(char aData){
	char buf[2];	
	data = aData;
	buf[0] = data; 
	::write(fd, buf, 1);
}
char I2CPort::read(){	
	char buf[2];	
	::read(fd, buf, 1);
	data = buf[0];
	return buf[0];

}

int main()
{
	I2CPort p1;
	p1.write(0xff);
	sleep(1);
	p1.write(0x00);
	sleep(1);
	p1.write(0xff);
	sleep(1);
	p1.write(0x00);
	sleep(1);
	p1.write(0xff);
	sleep(1);	
}
