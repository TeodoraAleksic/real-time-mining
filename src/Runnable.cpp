#include "stdafx.h"
#include "Runnable.h"


Runnable::Runnable(): running(false), thread()
{
}


Runnable::~Runnable()
{
	try { stop(); }
	catch (...) {}
}


bool Runnable::isRunning()
{
	return running;
}


void Runnable::start()
{
	if (!running)
	{
		running = true;
		thread = std::thread(&Runnable::run, this);
	}
}


void Runnable::stop()
{
	running = false;
	if (thread.joinable()) thread.join();
}
