

#include "ioport.h"
#include "iostream"
#include "stdio.h"



using namespace std;

I2CPort * I2CPort::myPtr = NULL;
uint8_t I2CPort::count = 0;

I2CPort * I2CPort::Create(){
	I2CPort::count ++;
	if(NULL == I2CPort::myPtr){
		I2CPort::myPtr = new I2CPort;
		DBG_PRINT("created");
	}
	return I2CPort::myPtr;
}
void I2CPort::Release()
{
	I2CPort::count--;
	if(0 == I2CPort::count){
		DBG_PRINT("I2CPort released");
		delete I2CPort::myPtr;
		I2CPort::myPtr = NULL;
	}
}
I2CPort::I2CPort(uint8_t aAdd ){
	fd = open("/dev/i2c/0",O_RDWR);

	if(fd < 0){
		DBG_PRINT("error opening device ");
		throw 1;
	}

	DBG_PRINT(" open device success ");

	if( ioctl(fd, I2C_SLAVE, aAdd) < 0){
		DBG_PRINT("error setting address");
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
bool I2CPort::getPin(uint8_t pinNumber){
	readPort();
	return data& (1 << pinNumber);
}
void I2CPort::setpin(bool state,uint8_t pinNumber)
{
//	readPort(); //data will update with latest port data
	data = state ? (data | 1 << pinNumber) : (data & ~(1 << pinNumber));
	setPort(data);
}

I2CPort::~I2CPort()
{
	DBG_PRINT("port destructor ");
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
