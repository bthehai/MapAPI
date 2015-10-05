#pragma once
#include "..\thread\Runnable.h"

class Task : public Runnable {
public:
	enum STATE {
		ERROR	= -1,
		CANCEL	= 0,
		GO		= 1,
		DONE	= 2,
	};

protected:
	int state;

	bool isCanceled;

public:
	Task();
	virtual ~Task();

public:
	virtual void run();
	virtual void cancel();

public:
	virtual int go(bool canceled) = 0;

};

Task::Task()
{
	state = GO;
	isCanceled = false;
}

Task::~Task()
{

}

void Task::run()
{
	go(state == CANCEL);
}

void Task::cancel()
{
	state = CANCEL;
}
