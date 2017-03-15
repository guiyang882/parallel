#include <iostream>
#include "threadpool.h"

const string logpath = "running.log";




int main() {
	Log::Initialise(logpath);
	Log::SetThreshold(Log::LOG_TYPE_INFO);
	PUSH_LOG_STACK;
	ThreadPool* p_threadPool = ThreadPool::getSingleInstance();
	p_threadPool->revokeSingleInstance();
	return 0;
}