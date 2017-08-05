#pragma once

#include <thread>

/// <summary>
/// A generic thread pool that executes whatever.
/// </summary>
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

private:

	/// <summary>
	/// The number of CPUs on the current machine.
	/// </summary>
	unsigned __int8 CPUCount;
	
	/// <summary>
	/// The threads we have to work with.
	/// </summary>
	vector<thread*>* Threads = new vector<thread*>();
};