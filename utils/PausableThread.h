#pragma once


class PausableThread {

private:
	bool m_bPausing;
	bool m_bRunning;
	bool m_bShouldPause;
	bool m_bShouldStop;

public:
	PausableThread();
	virtual ~PausableThread();

public:
	void awaitPausing();
	void finish();
	void pause();
	void proceed();

	bool isCanceled();	
	bool isPausing();
	bool isInterrupted();

	void notify();
	void start();
	
public:
	virtual void run();
	virtual int getThreadPriority();


protected:
	virtual void afterPause();
	virtual void afterRun();

	virtual void doWork() = 0;
	virtual char* getThreadName() = 0;
	virtual bool hasWork() = 0;

};