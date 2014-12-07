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

#include <termios.h>
#include <stdio.h>

static struct termios old_termios, new_termios;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
	tcgetattr(0, &old_termios); /* grab old terminal i/o settings */
	new_termios = old_termios; /* make new settings same as old settings */
	new_termios.c_lflag &= ~ICANON; /* disable buffered i/o */
	new_termios.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new_termios); /* use these new terminal i/o settings now */
}

/* Restore old_termios terminal i/o settings */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &old_termios);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
	return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
	return getch_(1);
}

void autoMode(Engine* pEngine)
{
	uint8_t speed = 100;
	I2CPort * port = I2CPort::Create();
	timespec tsR,tsE;
	pEngine->SetSpeed(speed);
	tsR.tv_sec 	= 0;//onTime/1000; //onTime is in millisecond
	tsR.tv_nsec = 10000000; //100 millisecond delay
	while(1)
	{
		uint8_t data = 0;
		nanosleep(&tsR,&tsE);
		data = port->readPort();
/*		cout<<( data & (1 << 0))<<( data & (1 << 1))<<( data & (1 << 2))<<
			( data & (1 << 3))<<( data & (1 << 4))<<
			( data & (1 << 5))<<( data & (1 << 6))<<( data & (1 << 7))<<endl;
*/

		if( data & (1 << 4))
		{

			cout<<"LIR is ON "<<endl;
			pEngine->SetDirection(false);
			pEngine->SetSpeed(50);
			sleep(1);
			pEngine->SetDirection(true);
			pEngine->MoveRight(200);
			sleep(1);
			pEngine->SetDirection(true);
			pEngine->SetSpeed(speed);
			pEngine->SetSpeed(speed);

		}
	}
}


int main()
{
	char but;
	uint8_t speed = 0;
	Engine* pEngine = new Engine();
	pEngine->SetSpeed(0);
	pEngine->Start();

	while(1)
	{
		//cin>>but;
		but = getch();
		switch(but)
		{
			case '8':
				pEngine->SpeedUp();
				break;
			case '2':
				pEngine->SpeedDown();
				break;
			case '4':
				pEngine->MoveLeft(100);
				break;
			case '6':
				pEngine->MoveRight(100);
				break;

			case '5':
				pEngine->SetSpeed(0);
				break;
			case 'a':
				autoMode(pEngine);
				break;
			default:
				return 0;
				pEngine->SetSpeed(0);


		}
	}
}
