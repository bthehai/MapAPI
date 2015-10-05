#pragma once

class TaskQueue;
class Runnable;

class AsyncExecutor {

private:
	TaskQueue *m_pMainLoop;
	//ExecutorService *m_pExecutor;

public:
	AsyncExecutor(int maxConcurrency, TaskQueue *pMainLoop);
	virtual ~AsyncExecutor();

public:
	bool post(Runnable *pTask);
	void dispose();
};

