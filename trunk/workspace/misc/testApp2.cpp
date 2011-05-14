/*
 * testApp2.cpp
 *
 *  Created on: 03-Apr-2011
 *      Author: renjith
 */

#include "engine.h"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
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
	Engine* pEngine = new Engine();

	buttons_fd = open("/dev/buttons", 0);

	if (buttons_fd < 0) {
		DBG_PRINT("open device buttons");
		throw;
	}
	pEngine->SetSpeed(0);
	pEngine->Start();
	while(1){
		but = getButton(buttons_fd);
		switch (but){
		case 1:{

			I2CPort * port = I2CPort::Create();
			timespec tsR,tsE;
			uint8_t LIRCount = 0,RIRCount = 0,PIRCount = 0;


			cout<<"pressed one"<<endl;
			sleep(3);
			speed =99;
			cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
			pEngine->SetSpeed(speed);

			tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
			tsR.tv_nsec = 10000000; //100 millisecond delay
			while(1){
				uint8_t data = 0;

				nanosleep(&tsR,&tsE);
				//sleep(1);
				data = port->readPort();
				//cout<<"port"<<(int)data<<"\n"<<endl;
				//============================================================
				//Left IR sensor
				if( data & (1 << 4)){

				//	cout<<"LIR triggered "<<endl;
					//cout<<"LIR changed"<<endl;
				//	if(LIRCount > 0){
						//LIR is ON
						cout<<"LIR is ON "<<endl;
						pEngine->SetDirection(false);
						pEngine->SetSpeed(20);
						sleep(1);
						pEngine->SetDirection(true);
						pEngine->MoveRight(200);
						sleep(1);
						pEngine->SetDirection(true);
						pEngine->SetSpeed(speed);
						pEngine->SetSpeed(speed);
				//	}
				//	else{
						//cout<<"LIRCount = "<<(int)LIRCount<<"\n";
				//		LIRCount ++;
				//	}

				}
				//else{
					//cout<<"LIRCount cleared \n";
				//	LIRCount = 0;
				//}

				//============================================================
				//Right IR sensor
				/*
				 * if( data & (1 << 5)){
					//cout<<"RIR changed"<<endl;
					if(RIRCount > 3){
						//LIR is ON
						cout<<"RIR is ON "<<endl;
						pEngine->SetDirection(false);
						sleep(1);
						pEngine->MoveLeft(200);
						pEngine->SetDirection(true);
						sleep(1);
						pEngine->SetSpeed(speed);
					}
					else{
						cout<<"RIRCount = "<<(int)RIRCount<<"\n";
						RIRCount ++;
					}

				}
				else{
					//cout<<"RIRCount cleared \n";
					RIRCount = 0;
				}*/
				//============================================================
				// Passive infrared sensor
				if( data & (1 << 6)){
					//cout<<"PIR changed"<<endl;
					if(PIRCount > 3){
						//LIR is ON
						cout<<"PIR is ON "<<endl;
					}
					else{
						//cout<<"PIRCount = "<<(int)PIRCount<<"\n";
						PIRCount ++;
					}

				}
				else{
					//cout<<"PIRCount cleared \n";
					PIRCount = 0;
				}

			}
			pEngine->SetSpeed(0);
			break;
		}
		case 2:
			cout<<"pressed two"<<endl;
			speed =100;

			cout<<"speed changed to " <<(int)speed<<"\n"<<endl;
			pEngine->SetSpeed(99);
			pEngine->SetDirection(true);
			pEngine->Start();
			while(1){
				int temp;
				cout<<"enter new angle"<<endl;
				scanf("%d",&temp);
				speed = (uint8_t)temp;
				cout<<"angle = "<<(int)speed<<"\n";

				pEngine->MoveRight(speed);
			}
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
