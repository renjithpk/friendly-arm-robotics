/*
 * stateMachine.cpp
 *
 *  Created on: 25-Feb-2015
 *      Author: renjith pk
 */

#include "stateMachine.h"
#include "roboApp.h"

/////////////////////// CONTEXT ///////////////////////
//CONTEXT
Context::Context()
{
	cState = NULL;
	setCurrentState(new RoboInit(*this));
	isTrack = false;
}

//CONTEXT
bool lastStateRight = true; 
bool movingRight = false;
void Context::startTracking()
{	
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	cState->handleInit();
	isTrack = true;
}
//CONTEXT
void Context::setCurrentState(State *stPtr)
{
	delete cState;
	cState = stPtr;
}


//CONTEXT
int Context::handleBallNone(EMessageT oType,int count)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	if(isTrack)
	{
		if( (NULL != cState) && (oType == DTD_OBJ_M_BALL))
		{
			cState->handleMultipleBall();
		}
		if( (NULL != cState) && (oType == DTD_OBJ_NOBALL))
		{
			cState->handleBallNotFound();
		}
	}
}



//CONTEXT
int Context::handleBallDetected(EMessageT oType,RCircle & rCircle)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	this->rCircle = rCircle;
	int err = getError(rCircle );
	syslog(LOG_INFO,"Err: %d",err);
	if(err >0)
	{
		if(NULL != cState) cState->handleBallOnRight(rCircle,err);
	}
	else if(err <0)
	{
		if(NULL != cState) cState->handleBallOnLeft(rCircle,err);
	}
	else
	{
		if(NULL != cState) cState->handleBallOnCenter(rCircle,err);
	}
}
//CONTEXT
int Context::getError(RCircle &data)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	/*
	center range x
	*/
	// 0 300 600
	//0     220   370  640

	//if(data.x < 370 && data.x > 220)
	const int xmax = 100;
	const int offset = 10;	
	int lowerLimit = ((xmax/2) - offset) - data.r;
	int upperLimit = ((xmax/2) + offset) + data.r;

	if( (data.x > lowerLimit)  && (data.x < upperLimit) )
	{
		return 0;
	}
	else if(data.x > upperLimit)
	{
		//return data.x - upperLimit;
		return data.x - xmax/2;
	}
	//return data.x - lowerLimit;
	return data.x - xmax/2;

}

void Context::setDirectionL(int err)
{	
	syslog(LOG_INFO, "State::%s ENTRY %d",__func__,err);
	Engine * engine = Engine::getInstance();
	int angle = err * -2;
	engine->MoveLeft(angle);
}

void Context::setDirectionR(int err)
{	
	syslog(LOG_INFO, "State::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	int angle = err * 2;
	engine->MoveRight(angle);
}
/////////////////////// STATE ///////////////////////
//STATE
State::State(Context &ctxt):context(ctxt)
{
}
State::~State()
{
}
int State::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);
}
int State::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);	
}
int State::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);	
}
int State::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);	
}
int State::handleMultipleBall()
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);	
}
int State::handleInit()
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);	
}
///////////////////////  ROBO INIT ///////////////////////

RoboInit::RoboInit(Context &ctxt):State(ctxt),isTurnedBack(false)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
    isRequestSendToconfirm = false;
}
RoboInit::~RoboInit()
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);	
	sstate = EStop;
}

int RoboInit::handleInit()
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	app_gp->reqRepObjDetet();
	sstate = ERotateH;
	setDelay(1);
}

void RoboInit::onTimerExpired()
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY, sstate [%d]  ",__func__,(int)sstate);
	Engine * engine = Engine::getInstance();
	switch(sstate)
	{
		case ERotateL: //0
		engine->SetSpeed(100);
		engine->MoveLeft(200);
		setDelay(200);
		sstate = ERotateH;
	    break;
		
		case ERotateH: //1
		engine->SetSpeed(20);
		engine->MoveLeft(200);
		setDelay(800);
		sstate = ERotateL;
		break;
		
		case EVerify: //2
		engine->SetDirection(true);
		engine->SetSpeed(0);
	    break;
		
		case EVerifyTB: //3
		handleInit();	
		break;
		case EStop: //4
		break;
	}
}

int RoboInit::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	if(sstate == EVerify)
	{	
		//turn back;
		engine->SetSpeed(20);
		engine->MoveRight(100);
		setDelay(300);
		sstate = EVerifyTB;//turning back
	}
}
bool RoboInit::confirmObject()
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	bool ret = false;
	Engine * engine = Engine::getInstance();
	switch(sstate)
	{
		case ERotateL:
		case ERotateH:
			{
				syslog(LOG_INFO, "confirm object found");
				engine->SetSpeed(0);
				app_gp->requestObj();
				sstate = EVerify;
				break;
			}
		case EVerifyTB:
		case EVerify:
			{
				syslog(LOG_INFO, "confirmed");
				ret = true;
				break;
			}
	}
	return ret;
}
int RoboInit::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	if(confirmObject())
	{
		context.setCurrentState(new BallOnRight(context));
		app_gp->reqRepObjDetet();
	}
}
int RoboInit::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	if(confirmObject())
	{
		context.setCurrentState(new BallOnLeft(context));
		app_gp->reqRepObjDetet();
	}
}
int RoboInit::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	if(confirmObject())
	{
		context.setCurrentState(new BallLocked(context));
		app_gp->reqRepObjDetet();
	}
}
int RoboInit::handleMultipleBall()
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	if(confirmObject())
	{
		context.setCurrentState(new BallOnRight(context));
		app_gp->reqRepObjDetet();
	}
}
/////////////////////// BALL ON RIGHT ///////////////////////

BallOnRight::BallOnRight(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "%s ENTRY",__func__);
	lastStateRight = true;
	Engine * engine_p = Engine::getInstance();
	engine_p->SetSpeed(100);
	engine_p->MoveRight(70);
}

BallOnRight::~BallOnRight()
{
}

int BallOnRight::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	Engine * engine_p = Engine::getInstance();
	context.setCurrentState(new RoboInit(context));
	engine_p->SetSpeed(0);
}

int BallOnRight::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	//engine_p->MoveRight(70);

	context.setDirectionR(err);
}

int BallOnRight::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	context.setCurrentState(new BallOnLeft(context));
}

int BallOnRight::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	context.setCurrentState(new BallLocked(context));
}
int BallOnRight::handleMultipleBall()
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	engine->SetSpeed(0);
}


//////////////////// BALL ON LEFT /////////////////////////////

BallOnLeft::BallOnLeft(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	lastStateRight = false;
	Engine * engine_p = Engine::getInstance();
	engine_p->SetSpeed(100);
	engine_p->MoveLeft(70);
}
BallOnLeft::~BallOnLeft()
{
}
int BallOnLeft::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	Engine * engine_p = Engine::getInstance();
	context.setCurrentState(new RoboInit(context));
	engine_p->SetSpeed(0);
}
int BallOnLeft::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.setCurrentState(new BallOnRight(context));
}
int BallOnLeft::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.setDirectionL(err);
}
int BallOnLeft::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.setCurrentState(new BallLocked(context));
}
int BallOnLeft::handleMultipleBall()
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
    engine->SetSpeed(0);
}

//////////////////////// BALL LOCKED  ////////////////////////////


BallLocked::BallLocked(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	if(ctxt.rCircle.r > 130 )
	{ 
		syslog(LOG_INFO,"Reached Ball");
		engine->SetSpeed(0);
	}
	else
	{
	    engine->SetSpeed(100);
	}
}
BallLocked::~BallLocked()
{
	syslog(LOG_INFO, "%s ENTRY",__func__);
}

int BallLocked::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "BallLocked::%s ENTRY",__func__);
	Engine *engine = Engine::getInstance();
	engine->SetSpeed(0);
}

int BallLocked::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	context.setCurrentState(new BallOnRight(context));
}

int BallLocked::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	context.setCurrentState(new BallOnLeft(context));
}

int BallLocked::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	Engine *engine = Engine::getInstance();
	engine->SetDirection(true);
	if(rCircle.r > 130 )
	{ 
		syslog(LOG_INFO,"Reached Ball");
		engine->SetSpeed(0);
	}
}

int BallLocked::handleMultipleBall()
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	Engine * engine = Engine::getInstance();
	engine->SetSpeed(0);
}
