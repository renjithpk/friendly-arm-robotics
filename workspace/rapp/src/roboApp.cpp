/*
 * RoboApp.cpp
 *
 *  Created on: 03-May-2011
 *      Author: renjith
 */

#include "roboApp.h"
#include "types.h"
#include "iostream"
using namespace std;
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
	cout<<"Press any key to continue.."<<endl;
	app.Register(&myapp);
	return 0;
}

RoboApp::RoboApp()
{
	mode =0;
//	syslog(LOG_DEBUG, "%s ENTRY",__func__);
}
/*
int RoboApp::onExit()
{
	//TODO	
}*/

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

int RoboApp::menu_2(int key)
{
	switch (key)
	{
		default:
				context.startTracking();
		break;	
		case 'x':
			mode = 0;
			menu_0('p');	
		break;
	}
	
}
int RoboApp::menu_3(int key)
{
	Engine * engine = Engine::getInstance();
	syslog(LOG_INFO, "%s ENTRY",__func__);
	switch((char) key)
	{
		default:
			cout<<" servo controll 4  6 8 2  "<<endl;
			break;
		case '4':
			engine->SpeedUp();
			break;
		case '6':
			engine->SpeedDown();
			break;
		case '8':
			engine->MoveLeft(70);
			break;
		case '2':
			engine->MoveRight(70);
			break;
		case 'x':
			mode = 0;
			menu_0('p');	
		break;
	}
	
}
int RoboApp::menu_0(int key)
{
	
	switch (key)
	{
		default:
		cout<<"Press 't' -> track ball, 'm' manual ctrl , 's' servo ctrl, 'c' caliberate, 'x' exit"<<endl;
		break;
		case 't':
		mode = 2;
		menu_2('p');
		break;
		case 'm':
		mode = 1;
		menu_1('p');
		break;
		case 's':
		mode = 3;
		menu_3('p');
		break;
		case 'c':
		mode = 4;
		menu_4('p');
		break;
		case 'x':
		app_gp->exit();
		break;
	}
	//std::cout<<"Press 'x' for exit"<<endl;	

}

int RoboApp::menu_4(int key)
{
	switch(key)
	{
		default:	
			cout<<"Press 'c' -> caliberate, 's'  stop"<<endl;
			break;
		case 'c':
			syslog(LOG_INFO,"caliberate ");
			app_gp->reqRepObjDetet();
			break;
		case 's':
			syslog(LOG_INFO,"stop caliberating ");
			app_gp->stopRepObjDetet();
			break;
		case 'x':
			mode = 0;
			menu_0('p');	
		break;
	}

}
//0 -- main menu
int RoboApp::onKeyPressed(int ch)
{
	switch (mode)
	{
		case 0:
		menu_0(ch);
		//onKeyPressed(ch);
		break;
		case 1:
		menu_1(ch);
		break;
		case 2:
		menu_2(ch);
		break;
		case 3:
		menu_3(ch);
		break;
		case 4:
		menu_4(ch);
		break;
	}
}
int RoboApp::menu_1(int key)
{
	Engine * engine = Engine::getInstance();
	syslog(LOG_INFO, "%s ENTRY",__func__);
	switch((char) key)
	{
		default:
			cout<<"4 5 6 8 2 a A z Z "<<endl;
			break;
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
			mode = 0;
			menu_0('p');	
		break;
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

