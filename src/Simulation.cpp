#include "Simulation.h"

#include "GlobalException.h"


Simulation::Simulation(): Sleepable(200), pc(em, wt), wlm(wt, pc)
{
}


Simulation::~Simulation()
{
	stop();
}


void Simulation::run()
{
	em.start();
	wt.start();
	pc.start();
	wlm.start();

	while (running)
	{
		computeNextTime();
		std::this_thread::sleep_until(sleepUntil);

		// Ends simulation if any thread set a global exception
		if (GlobalException::getInstance().getGlobalException())
			running = false;
	}

	wlm.stop();
	pc.stop();
	wt.stop();
	em.stop();
}
