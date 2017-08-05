#include "stdafx.h"
#include "ThreadPool.h"


ThreadPool::ThreadPool()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	this->CPUCount = sysinfo.dwNumberOfProcessors;
	Console->WriteLine(&(to_string(this->CPUCount) + " CPUs detected!"));
}


ThreadPool::~ThreadPool()
{
}
