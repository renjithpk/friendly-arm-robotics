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
}

//CONTEXT
void Context::setCurrentState(State *stPtr)
{
	delete cState;
	cState = stPtr;
}

// DTD_OBJ_FACE,
// DTD_OBJ_NOBALL,
// DTD_OBJ_BALL,
// DTD_OBJ_M_BALL

//CONTEXT
int Context::handleBallNone(EMessageT oType,int count)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	if( (NULL != cState) && (oType == DTD_OBJ_M_BALL))
	{
		cState->handleMultipleBall();
	}
	if( (NULL != cState) && (oType == DTD_OBJ_NOBALL))
	{
		cState->handleBallNotFound();
	}
	//handleBallNotFound(count);
}



//CONTEXT
int Context::handleBallDetected(EMessageT oType,RCircle & rCircle)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
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
void Context::startTracking()
{	
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	app_gp->requestObj();//TODO change name 
}

//CONTEXT
int Context::getError(RCircle &data)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	/*
	center range x
	*/
	// 0 300 600
	//0     220   370  600

	//if(data.x < 370 && data.x > 220)
	int lowerLimit = 250 - data.r;
	int upperLimit = 350 + data.r;
	if( (data.x > lowerLimit)  && (data.x < upperLimit) )
	{
		return 0;
	}
	else if(data.x > upperLimit)
	{
		return 1;
	}
	return -1;

}


//CONTEXT
void Context::rotate(char dir)
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	Engine * engine_p = Engine::getInstance();
	engine_p->SetSpeed(20);
	if(dir == 'R') engine_p->MoveRight(200);
	else if(dir == 'L') engine_p->MoveLeft(200);
	else syslog(LOG_ERR,"Invalid direction : %c ",dir);	
}

//CONTEXT
void  Context::moveRight()
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	Engine * engine_p = Engine::getInstance();
	engine_p->SetSpeed(20);
	engine_p->MoveRight(200);
	sleep(1);
	engine_p->SetSpeed(0);
	sleep(1);
}

//CONTEXT
void  Context::moveLeft()
{
	syslog(LOG_INFO, "Context::%s ENTRY",__func__);
	Engine * engine_p = Engine::getInstance();
	engine_p->SetSpeed(20);
	engine_p->MoveLeft(200);
	sleep(1);
	engine_p->SetSpeed(0);
	sleep(1);
}

/////////////////////// STATE ///////////////////////
//STATE
State::State(Context &ctxt):context(ctxt)
{
}
State::~State()
{
}
/*
int State::handleBallDetected(RCircle & rCircle)
{
	syslog(LOG_INFO, "State::%s ENTRY",__func__);
}*/
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
///////////////////////  ROBO INIT ///////////////////////

RoboInit::RoboInit(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
}
RoboInit::~RoboInit()
{
}
/*
int RoboInit::handleBallDetected(RCircle & rCircle)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	app_gp->requestObj();//TODO change name 
}*/
int RoboInit::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	context.rotate('R');
	sleep(1);
	app_gp->requestObj();
}
int RoboInit::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	context.moveRight();	
	context.setCurrentState(new BallOnRight(context));
	app_gp->requestObj();
}
int RoboInit::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	context.moveLeft();	
	context.setCurrentState(new BallOnLeft(context));
	app_gp->requestObj();
}
int RoboInit::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	context.setCurrentState(new BallLocked(context));
	app_gp->requestObj();
}
int RoboInit::handleMultipleBall()
{
	syslog(LOG_INFO, "RoboInit::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	engine->SetDirection(false);
	engine->SetSpeed(30);
	sleep(1);
	engine->SetDirection(true);
	engine->SetSpeed(0);
	app_gp->requestObj();
}
/////////////////////// BALL ON RIGHT ///////////////////////

BallOnRight::BallOnRight(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "%s ENTRY",__func__);
}

BallOnRight::~BallOnRight()
{
}
int BallOnRight::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	context.rotate('R');
	sleep(1);
	app_gp->requestObj();

}
int BallOnRight::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	context.moveRight();	
	app_gp->requestObj();
}
int BallOnRight::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	context.moveLeft();	
	context.setCurrentState(new BallOnLeft(context));
	app_gp->requestObj();
}
int BallOnRight::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	context.setCurrentState(new BallLocked(context));
	app_gp->requestObj();
}
int BallOnRight::handleMultipleBall()
{
	syslog(LOG_INFO, "BallOnRight::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	engine->SetDirection(false);
	engine->SetSpeed(30);
	sleep(1);
	engine->SetDirection(true);
	engine->SetSpeed(0);
	app_gp->requestObj();
}


//////////////////// BALL ON LEFT /////////////////////////////

BallOnLeft::BallOnLeft(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
}
BallOnLeft::~BallOnLeft()
{
}
int BallOnLeft::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.rotate('L');
	sleep(1);
	app_gp->requestObj();
}
int BallOnLeft::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.moveRight();	
	context.setCurrentState(new BallOnRight(context));
	app_gp->requestObj();
}
int BallOnLeft::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.moveLeft();	
	app_gp->requestObj();
}
int BallOnLeft::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	context.setCurrentState(new BallLocked(context));
	app_gp->requestObj();
}
int BallOnLeft::handleMultipleBall()
{
	syslog(LOG_INFO, "BallOnLeft::%s ENTRY",__func__);
	Engine * engine = Engine::getInstance();
	engine->SetDirection(false);
	engine->SetSpeed(30);
	sleep(1);
	engine->SetDirection(true);
	engine->SetSpeed(0);
	app_gp->requestObj();
}

//////////////////////// BALL LOCKED  ////////////////////////////


BallLocked::BallLocked(Context &ctxt):State(ctxt)
{
	syslog(LOG_INFO, "%s ENTRY",__func__);
}
BallLocked::~BallLocked()
{
	syslog(LOG_INFO, "%s ENTRY",__func__);
}

int BallLocked::handleBallNotFound(int count)
{
	syslog(LOG_INFO, "BallLocked::%s ENTRY",__func__);
	Engine *engine = Engine::getInstance();
	engine->SetDirection(false);
	engine->SetSpeed(100);
	sleep(1);
	engine->SetSpeed(0);
	context.moveRight();	
	context.setCurrentState(new BallOnRight(context));	
	app_gp->requestObj();//TODO change name
}
int BallLocked::handleBallOnRight(RCircle &rCircle,int err)
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	context.moveRight();	
	context.setCurrentState(new BallOnRight(context));
	app_gp->requestObj();
}
int BallLocked::handleBallOnLeft(RCircle &rCircle,int err)
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	context.moveLeft();	
	context.setCurrentState(new BallOnLeft(context));
	app_gp->requestObj();
}
int BallLocked::handleBallOnCenter(RCircle &rCircle,int err)
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	Engine *engine = Engine::getInstance();
	engine->SetDirection(true);
	if(rCircle.r > 130 )
	{ 
		syslog(LOG_INFO,"Go and hit ball");
		engine->SetSpeed(100);
		sleep(3);
	}
	engine->SetSpeed(20);
	app_gp->requestObj();
}
int BallLocked::handleMultipleBall()
{
	syslog(LOG_INFO,"BallLocked::%s ",__func__);
	Engine * engine = Engine::getInstance();
	engine->SetDirection(false);
	engine->SetSpeed(30);
	sleep(1);
	engine->SetDirection(true);
	engine->SetSpeed(100);
	sleep(1);
	engine->SetSpeed(0);
	app_gp->requestObj();
}
