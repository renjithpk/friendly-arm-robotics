/*
 * stateMachine.h
 *
 *  Created on: 25-Feb-2015
 *      Author: renjith
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
#include "cvInterface.h"
#include "engine.h"
//#include "Context.h"
class State;


class Context
{
	public:
	Context();
	State*  cState;
	void setCurrentState(State *stPtr);
	int handleBallDetected(EMessageT oType, RCircle &rCircle);
	int handleBallNotFound(EMessageT oType, int count);
	int getError(RCircle &data);
	void startTracking();
	//TODO move this to engine
	void rotate(char dir);
	void moveRight();
	void moveLeft();
};


class State
{
//	Engine* engine;
public:
	Context & context;
	State(Context & ctxt);
	virtual ~State();
//	virtual int handleTrackBall();
	virtual int handleBallDetected(RCircle &rCircle);
	virtual int handleBallNotFound(int count);
	virtual int handleBallOnRight(RCircle &rCircle,int err);
	virtual int handleBallOnLeft(RCircle &rCircle,int err);
	virtual int handleBallOnCenter(RCircle &rCircle,int err);
	virtual int handleMultipleBall(RCircle &rCircle,int err);
};

class BallLocked: public State
{
public:
	BallLocked(Context &ctxt);
	virtual ~BallLocked();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleMultipleBall(RCircle &rCircle,int err);
};

class BallOnLeft: public State
{
public:
	BallOnLeft(Context &ctxt);
	virtual ~BallOnLeft();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleMultipleBall(RCircle &rCircle,int err);
};

class BallOnRight: public State
{
public:
	BallOnRight(Context &ctxt);
	virtual ~BallOnRight();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleMultipleBall(RCircle &rCircle,int err);
};


class RoboInit: public State
{
public:
	RoboInit(Context &ctxt);//:State(ctxt) {}
	virtual ~RoboInit();
	//virtual int handleTrackBall();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleMultipleBall(RCircle &rCircle,int err);
};

#endif /* STATEMACHINE_H_ */
