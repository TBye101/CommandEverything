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
	template<class Fn, class... Args>
	auto Go(Fn c, Args... args) = 0;
};