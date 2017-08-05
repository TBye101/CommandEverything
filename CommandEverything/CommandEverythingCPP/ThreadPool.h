#pragma once

#include <thread>

/// <summary>
/// A simple thread pool implementation.
/// </summary>
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

private:
	/// <summary>
	/// Our threads.
	/// </summary>
	vector<thread*>* Threads = new vector<thread*>();
};

/// <summary>
/// A basic task.
/// </summary>
class ITask
{
	virtual void Go(__int8 argCount, ...) = 0;
};