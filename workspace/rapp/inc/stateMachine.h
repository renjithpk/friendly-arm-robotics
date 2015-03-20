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
#include "rtimer.h"

class State;


class Context
{
	public:
	Context();
	State*  cState;
	void setCurrentState(State *stPtr);
	int handleBallDetected(EMessageT oType, RCircle &rCircle);
	int handleBallNone(EMessageT oType, int count = -1);
	int getError(RCircle &data);
	void startTracking();
	void rotate(char dir);
	void moveRight();
	void moveLeft();
};


class State
{
public:
	Context & context;
	State(Context & ctxt);
	virtual ~State();
	virtual int handleBallOnRight(RCircle &rCircle,int err);
	virtual int handleBallOnLeft(RCircle &rCircle,int err);
	virtual int handleBallOnCenter(RCircle &rCircle,int err);
	virtual int handleBallNotFound(int count = -1);
	virtual int handleMultipleBall();
};

class BallLocked: public State
{
public:
	BallLocked(Context &ctxt);
	virtual ~BallLocked();
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleBallNotFound(int count);
	int handleMultipleBall();
};

class BallOnLeft: public State
{
public:
	BallOnLeft(Context &ctxt);
	virtual ~BallOnLeft();
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleBallNotFound(int count);
	int handleMultipleBall();
};

class BallOnRight: public State
{
public:
	BallOnRight(Context &ctxt);
	virtual ~BallOnRight();
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleBallNotFound(int count);
	int handleMultipleBall();
};


class RoboInit: public State//, public RTimer
{
public:
	RoboInit(Context &ctxt);//:State(ctxt) {}
	virtual ~RoboInit();
	int handleBallOnRight(RCircle &rCircle,int err);
	int handleBallOnLeft(RCircle &rCircle,int err);
	int handleBallOnCenter(RCircle &rCircle,int err);
	int handleBallNotFound(int count);
	int handleMultipleBall();
};

#endif /* STATEMACHINE_H_ */
