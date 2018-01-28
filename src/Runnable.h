#ifndef _RUNNABLE_H
#define _RUNNABLE_H

#include <atomic>
#include <thread>

using namespace std;

class Runnable
{
protected:

	std::atomic<bool> running;

	std::thread thread;

	virtual void run() = 0;

public:

	Runnable();
	virtual ~Runnable();

	Runnable(Runnable const&) = delete;
	Runnable& operator =(Runnable const&) = delete;

	bool isRunning();

	void start();
	virtual void stop();

};

#endif
