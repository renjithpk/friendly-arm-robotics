/*
 * RoboApp.cpp
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#include "roboApp.h"
#include "types.h"

App * app_gp = NULL;
void init_log()
{
	    setlogmask(LOG_UPTO(LOG_INFO));
	    //setlogmask(LOG_UPTO(LOG_DEBUG));
		    openlog("rapp",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

int main(int argc,char *argv[])
{
	init_log();
	syslog(LOG_NOTICE, "Roboapp started!!%s","test");
	
	RoboApp  myapp;
	
	App app(argc,argv); app_gp = &app;

	app.Register(&myapp);
	return 0;
}

RoboApp::RoboApp()
{
//	syslog(LOG_DEBUG, "%s ENTRY",__func__);
}

RoboApp::~RoboApp() {
	// TODO Auto-generated destructor stub
}

int RoboApp::init(int argc,char *argv[])
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	return 0;
}


int RoboApp::onNewObject(EMessageT oType,void * data)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(oType == DTD_OBJ_BALL)
	{
		RCircle *rCircle =( RCircle *) data;
		syslog(LOG_DEBUG,"onNewObject() found  x:%d y:%d r:%d ",rCircle->x,rCircle->y,rCircle->r);
		context.handleBallDetected(oType,*rCircle);
	}
	else
	{
		context.handleBallNotFound(oType,0);
		syslog(LOG_DEBUG,"onNewObject() object not found ");
	}
}




int RoboApp::onKeyPressed(int ch)
{
	Engine * engine = Engine::getInstance();
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
				//track(false);
				context.startTracking();
				//app_p->requestObj();//TODO change name
			break;
			default:
				return 0;
				engine->SetSpeed(0);


		}
}





/*
bool validate(RCircle &rc_)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(rc_.y < 320 && rc_.y > 250)
	{
		return true;	
	}
	return false;
}
*/



/*
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
*/

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

