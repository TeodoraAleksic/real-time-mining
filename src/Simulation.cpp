#include "Simulation.h"

#include "GlobalException.h"


Simulation::Simulation(): Loggable("simulation"), Sleepable(100), pc(em, wt), wlm(wt, pc)
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


void Simulation::setSensorThreshold(EnvironmentMonitor::SensorID sensorID, double threshold)
{
	em.setThreshold(sensorID, threshold);
}


void Simulation::getSensorData(EnvironmentMonitor::SensorID sensorID, double& value, bool& alarm)
{
	em.getSensorData(sensorID, value, alarm);
}


bool Simulation::getPumpAlarm()
{
	return pc.getPumpAlarm();
}


bool Simulation::isPumpOn()
{
	return pc.isTurnedOn();
}


void Simulation::turnPumpOn()
{
	pc.turnOn();
}


void Simulation::turnPumpOff()
{
	pc.turnOff();
}


void Simulation::getWaterLevelAlarms(bool& highLevelAlarm_, bool& lowLevelAlarm_)
{
	wlm.getAlarms(highLevelAlarm_, lowLevelAlarm_);
}


void Simulation::setHighLevelThreshold(double threshold)
{
	wlm.setHighLevelThreshold(threshold);
}


void Simulation::setLowLevelThreshold(double threshold)
{
	wlm.setLowLevelThreshold(threshold);
}


double Simulation::getWaterLevel()
{
	return wt.getWaterLevel();
}


void Simulation::changeWaterSpeed(double speed)
{
	wt.changeWaterSpeed(speed);
}
