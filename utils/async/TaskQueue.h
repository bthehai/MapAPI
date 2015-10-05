#pragma once

class Runnable;

class TaskQueue {
public:
	virtual bool post(Runnable *pTask) = 0;
	virtual void addTask(Runnable *pTask) = 0;
};