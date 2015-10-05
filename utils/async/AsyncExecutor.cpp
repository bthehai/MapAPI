#include "stdafx.h"
#include "AsyncExecutor.h"
#include "TaskQueue.h"
#include "..\thread\Runnable.h"

AsyncExecutor::AsyncExecutor( int maxConcurrency, TaskQueue *pMainLoop )
{
}

AsyncExecutor::~AsyncExecutor()
{
}

bool AsyncExecutor::post( Runnable *pTask )
{
	throw std::exception("The method or operation is not implemented.");
}

void AsyncExecutor::dispose()
{
	throw std::exception("The method or operation is not implemented.");
}
