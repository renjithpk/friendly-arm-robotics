/*
 * RoboApp.cpp
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#include "RoboApp.h"

App *app_p;
Engine * engine_p;
void init_log()
{
	    setlogmask(LOG_UPTO(LOG_DEBUG));
		    openlog("rapp",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

int main(int argc,char *argv[])
{
	init_log();
	syslog(LOG_NOTICE, "Roboapp started!!%s","test");
	App app(argc,argv);
	RoboApp  myapp;
	app_p = &app;
	app.Register(&myapp);
	return 0;
}

RoboApp::RoboApp()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	// TODO Auto-generated constructor stub
	engine 	= NULL; //instantiated inside initiazer
	speed 	= 0;
	isBlocked = false;
}

RoboApp::~RoboApp() {
	// TODO Auto-generated destructor stub
	delete engine;
}

int RoboApp::init(int argc,char *argv[])
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
//	PerDetect pd;
//	pd.initialize();
	engine = new Engine();
	engine_p = engine;
	engine->SetSpeed(0);
	engine->Start();
	engine->SetSpeed(0);
	sleep(5);
	engine->SetDirection(true);
	return 0;
}
bool validate(RCircle &rc_)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(rc_.y < 320 && rc_.y > 250)
	{
		return true;	
	}
	return false;
}

int getError(RCircle &data)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	/*
	center range x
	*/
	if(data.x < 370 && data.x > 220)
	{	
		return 0;	
	}
	else if(data.x > 350)
	{
		return 1;
	}
	return -1;
	
}
void rotate()
{	
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	engine_p->SetSpeed(20);
	engine_p->MoveRight(200);
}

void moveRight()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	engine_p->SetSpeed(20);
	engine_p->MoveRight(200);
	sleep(1);
	engine_p->SetSpeed(0);
	sleep(1);
}
void moveLeft()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	engine_p->SetSpeed(20);
	engine_p->MoveLeft(200);
	sleep(1);
	engine_p->SetSpeed(0);
	sleep(1);
}
#define ST_INIT 0
#define ST_ROTATE 1
#define ST_FOUND 2
#define ST_ADJUST_POS 3
#define ST_ADJUSTED 4
#define ST_ADJUSTERR 5
int rBstate = ST_INIT;
void track(bool isObj, void *data = NULL)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	switch (rBstate)
	{
		case ST_INIT:
			{
				syslog(LOG_INFO,"stateInit");
				if(!isObj)
				{
					rotate();
					rBstate = ST_ROTATE; 
					app_p->requestObj();//TODO change name
				}
				else
				{
					rBstate = ST_FOUND;
					engine_p->SetSpeed(0);
					app_p->requestObj();//TODO change name
				}
			}
			break;
		case ST_ROTATE:
			{
				syslog(LOG_INFO,"state ROtate");
				if(isObj)
				{
					rBstate = ST_FOUND;
					engine_p->SetSpeed(0);
					track(isObj,data);
				}
				app_p->requestObj();//TODO change name 
			}
			break;
		case ST_FOUND:
			{
				syslog(LOG_INFO,"state Found");
				if(!isObj)
				{
					rBstate = ST_INIT;
					app_p->requestObj();
					break;
				}
				if(NULL == data)
				{
					syslog(LOG_ERR,"data NULL");
					app_p->requestObj();//TODO change name 
				break;
				}
				int err = getError(*((RCircle *)data) );
				syslog(LOG_INFO,"Err: %d",err);
				if(err >0)
				{
					moveRight();	
					app_p->requestObj();//TODO change name 
				}
				else if(err <0)
				{
					moveLeft();	
					app_p->requestObj();//TODO change name 
				}
				else
				{
					syslog(LOG_INFO,"state Found ADJUSTED ******");
					rBstate = ST_ADJUSTED;
					app_p->requestObj();//TODO change name 
					//track(isObj,data);
				}
			}
			break;
		case ST_ADJUSTED:
		{
			if(!isObj)
			{
				rBstate = ST_INIT;
				app_p->requestObj();
				break;
			}
			
			if(NULL == data)
			{
				syslog(LOG_ERR,"data NULL");
				app_p->requestObj();//TODO change name 
				break;
			}
			
			int err = getError(*((RCircle *)data) );

			if(err != 0) 
			{
					syslog(LOG_INFO,"ADJUST error ******");
					rBstate = ST_FOUND;
					engine_p->SetSpeed(0);
					track(isObj,data);
			}else
			{
					syslog(LOG_INFO,"state Found ADJUSTED ******");
					rBstate = ST_ADJUSTED;	
					app_p->requestObj();//TODO change name 
			}
		//	rBstate = ST_ADJUSTERR;
		}
		break;
//		case ST_ADJUSTERR:
//		{
//			}
//		}
//		break;
	}
}

int RoboApp::onNewObject(EMessageT oType,void * data)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(oType == DTD_OBJ_BALL)
	{	
		RCircle *rCircle =( RCircle *) data;
		syslog(LOG_DEBUG,"onNewObject() found  x:%d y:%d r:%d ",rCircle->x,rCircle->y,rCircle->r);
		track(true,data);
	}
	else
	{

		track(false);
		syslog(LOG_DEBUG,"onNewObject() object not found "); 		
	}
	//app_p->requestObj();//TODO change name 
	//	MSG_Circle msg;
	//	msg.header.type = DTD_OBJ_BALL;
	//	msg.rCircle = temp;
}



int RoboApp::onKeyPressed(int ch)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
		switch((char) ch)
		{
			case '8':
			case 'a':
			case 'A':
				engine->SpeedUp();
				break;
			case '2':
			case 'z':
			case 'Z':
				engine->SpeedDown();
				break;
			case '4':
				engine->MoveLeft(100);
				break;
			case '6':
				engine->MoveRight(100);
				break;

			case '5':
				engine->SetSpeed(0);
				break;
			case 'x':
			    //ExitApp();
				//autoMode(engine);
				break;
			case 'r':
				track(false);
				//app_p->requestObj();//TODO change name 
			break;
			default:
				return 0;
				engine->SetSpeed(0);


		}
} 
/*
//Ir sensor ESensor_IR, state - true/false true => blocked
bool RoboApp::EventCallBack(ESensor aType, bool state)
{
	PRINT_1("EventCallBack [%d]", state);
	if( aType == ESensor_IR && state == true  ){
		
		engine->SetDirection(false);
		engine->SetSpeed(70);
	}
	else if( aType == ESensor_IR && state == false  ){
		sleep(1);
		engine->SetDirection(true);
		engine->MoveRight(100);
		sleep(1);
		engine->SetDirection(true);
		engine->SetSpeed(100);
	}

}
*/

