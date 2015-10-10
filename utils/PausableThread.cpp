#include "stdafx.h"
#include "PausableThread.h"

PausableThread::PausableThread()
	: m_bPausing(true)
	, m_bRunning(true)
	, m_bShouldPause(false)
	, m_bShouldStop(false)
{

}

PausableThread::~PausableThread()
{

}

void PausableThread::awaitPausing()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::finish()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::pause()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::proceed()
{
	throw std::exception("The method or operation is not implemented.");
}

bool PausableThread::isCanceled()
{
	throw std::exception("The method or operation is not implemented.");
}

bool PausableThread::isPausing()
{
	throw std::exception("The method or operation is not implemented.");
}

bool PausableThread::isInterrupted()
{
	throw std::exception("The method or operation is not implemented.");
}

int PausableThread::getThreadPriority()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::run()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::afterPause()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::afterRun()
{
	throw std::exception("The method or operation is not implemented.");
}

void PausableThread::notify()
{

}
