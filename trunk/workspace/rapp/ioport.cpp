#define DBG_ON

#include "ioport.h"
#include "iostream"
#include "stdio.h"


using namespace std;

I2CPort & I2CPort::Create(){
	static I2CPort myPtr;
	return myPtr;
}

I2CPort::I2CPort(uint8_t aAdd ){
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
void I2CPort::setPort(uint8_t aData){
	data = aData;
	::write(fd, &data, 1);
}
uint8_t I2CPort::readPort(){
	::read(fd, &data, 1);
	return data;
}
void I2CPort::setpin(bool state,uint8_t pinNumber)
{
	readPort(); //data will update with latest port data
	data = state ? (data | 1 << pinNumber) : (data & ~(1 << pinNumber));
	setPort(data);
}

I2CPort::~I2CPort()
{
	::close(fd);
}

#ifdef DBG_ON
	void I2CPort::display(){
		uint8_t mask = 1 << 7;
		cout<<"\n data : ";
		while(mask){
			if(data & mask) cout<<"1 ";
			else cout<<"0 ";
			mask = mask >> 1;
		}
		cout<<endl;
	}
#else
	void I2CPort::display(){}
#endif
/*
int main()
{
	I2CPort p1 = I2CPort::Create();
	uint8_t mask = 1;
	while(mask){
		p1.write(mask);
		p1.display();
		mask = mask << 1;
		sleep(2);
	}

	p1.setpin(1,0);
	sleep(2);
	p1.read();
	p1.display();
	p1.setpin(0,0);
	sleep(2);
	p1.read();
	p1.display();
	p1.write(0xaa);
	p1.setpin(0,1);
	sleep(2);
	p1.read();
	p1.display();
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
	uint8_t data = p1.read();
	cout<<"data : "<<data<<endl;
	sleep(5);
	data = p1.read();
	p1.display();
	sleep(5);
	cout<<"writing 0xaa"<<endl;
	p1.write(0xaa);
	p1.display();
	sleep(5);
	data = p1.read();
	p1.display();
	sleep(5);
	cout<<"writing 0xff"<<endl;
	p1.write(0xff);
	sleep(5);
	data = p1.read();
	p1.display();
	sleep(5);
	data = p1.read();
	p1.display();
	sleep(5);
	cout<<"terminating"<<endl;
}
*/
