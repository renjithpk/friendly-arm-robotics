/*
 * testApp2.cpp
 *
 *  Created on: 03-Apr-2011
 *      Author: renjith
 */

#include "engine.h"
#include <unistd.h>
#include <iostream>
using namespace std;


char getButton(int buttons_fd)
{

	char buttons[6] = {'0', '0', '0', '0', '0', '0'};
	char i;
	const char off = '0';
	char key = 0;

	do{
		key = 0;
		if (read(buttons_fd, buttons, sizeof buttons) != sizeof buttons) {
			DBG_PRINT("error:read buttons:");

		}

		for (i = 0; i <  sizeof buttons / sizeof buttons[0]; i++) {
			if(buttons[i] != off ){
				key = i + 1;
				break;
			}
			cout<<buttons[i];
		}
		cout<<endl;
	}while(!key);

	if (read(buttons_fd, buttons, sizeof buttons) != sizeof buttons) {
		DBG_PRINT("error:read buttons:");

	}

	return key;

}

int main(){
	int buttons_fd;
	char but;
	uint8_t speed = 0;
	engine* pEngine = new engine();

	buttons_fd = open("/dev/buttons", 0);

	if (buttons_fd < 0) {
		DBG_PRINT("open device buttons");
		throw;
	}

	while(1){
		but = getButton(buttons_fd);
		switch (but){
		case 1:
			cout<<"pressed one"<<endl;
			sleep(3);
			speed =1;
			cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
			pEngine->SetSpeed(speed);
			pEngine->Start();
			sleep(3);
			speed =5;
			cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
			pEngine->SetSpeed(speed);
			sleep(3);
			speed =20;
			cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
			pEngine->SetSpeed(speed);

			while(150 > speed){
				sleep(3);
				speed +=20;
				cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
				pEngine->SetSpeed(speed);

			}
			pEngine->SetSpeed(0);
			break;
		case 2:
			cout<<"pressed two"<<endl;
			speed =100;
			cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
			pEngine->SetSpeed(speed);
			pEngine->SetDirection(true);
			pEngine->Start();
			sleep(5);
			pEngine->SetDirection(false);
			sleep(5);

			pEngine->SetDirection(true);

			pEngine->MoveLeft(50);
			sleep(3);

			pEngine->MoveRight(50);
			sleep(3);

			pEngine->MoveLeft(150);
			sleep(1);

			pEngine->MoveRight(150);
			sleep(1);
			pEngine->SetDirection(false);
			pEngine->SetSpeed(speed);
			sleep(10);
			pEngine->SetSpeed(0);

			break;
		case 6:
			pEngine->SetSpeed(0);
			delete pEngine;
			system("poweroff");
			return 0;
			break;
		default:
			delete pEngine;
			return 0;
			cout<<"Default"<<endl;

		}
	}


}
