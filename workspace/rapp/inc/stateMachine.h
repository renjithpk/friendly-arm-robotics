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
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
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
};

class BallLocked: public State
{
public:
	BallLocked(Context &ctxt);
	virtual ~BallLocked();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
};

class BallOnLeft: public State
{
public:
	BallOnLeft(Context &ctxt);
	virtual ~BallOnLeft();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
};

class BallOnRight: public State
{
public:
	BallOnRight(Context &ctxt);
	virtual ~BallOnRight();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
};


class RoboInit: public State
{
public:
	RoboInit(Context &ctxt);//:State(ctxt) {}
	virtual ~RoboInit();
	//virtual int handleTrackBall();
	int handleBallDetected(RCircle &rCircle);
	int handleBallNotFound(int count);
};

#endif /* STATEMACHINE_H_ */
