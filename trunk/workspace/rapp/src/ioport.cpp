

#include "ioport.h"
#include "iostream"
#include "stdio.h"



using namespace std;

I2CPort * I2CPort::myPtr = NULL;
uint8_t I2CPort::count = 0;
pthread_mutex_t I2C_mtx;

I2CPort * I2CPort::Create(){
	I2CPort::count ++;
	if(NULL == I2CPort::myPtr){
		I2CPort::myPtr = new I2CPort;
		syslog(LOG_INFO,"created");
	}
	return I2CPort::myPtr;
}
void I2CPort::Release()
{
	I2CPort::count--;
	if(0 == I2CPort::count){
		syslog(LOG_INFO,"I2CPort released");
		delete I2CPort::myPtr;
		I2CPort::myPtr = NULL;
	}
}
I2CPort::I2CPort(uint8_t aAdd ){
	fd = open("/dev/i2c-0",O_RDWR);
	if(fd < 0)
	{
		syslog(LOG_ERR,"/dev/i2c-0  error opening device ");
		fd = open("/dev/i2c-1",O_RDWR);
		if(fd < 0)
		{
			syslog(LOG_ERR,"/dev/i2c-1 error opening device ");
		throw 1;
		}
	}

	syslog(LOG_INFO," open device success ");

	if( ioctl(fd, I2C_SLAVE, aAdd) < 0){
		syslog(LOG_ERR,"error setting address");
		throw 1;
	}

}
void I2CPort::setPort(uint8_t aData){
	data = aData;
	//mutex start
	pthread_mutex_lock(&I2C_mtx);
	::write(fd, &data, 1);
	pthread_mutex_unlock(&I2C_mtx);
	//mutex end
}
uint8_t I2CPort::readPort(){
	pthread_mutex_lock(&I2C_mtx);
	::read(fd, &data, 1);
	pthread_mutex_unlock(&I2C_mtx);
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
	syslog(LOG_DEBUG,"i2c data %x",data);
}

I2CPort::~I2CPort()
{
	syslog(LOG_INFO,"port destructor ");
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
