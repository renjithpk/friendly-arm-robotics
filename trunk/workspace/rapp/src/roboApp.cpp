/*
 * RoboApp.cpp
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#include "roboApp.h"
#include "types.h"
#include "iostream"
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
	std::cout<<"Press 'x' for exit"<<endl;	
	std::cout<<"up-8 or a, down-2 or z,left-4, right-6, stop-5 "<<endl;	
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
		context.handleBallNone(oType,0);
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
				engine->MoveLeft(70);
				break;
			case '6':
				engine->MoveRight(70);
				break;

			case '5':
				engine->SetSpeed(0);
				break;
			case 'x':
			    app_gp->exit();
				break;
			case 'r':
				context.startTracking();
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

