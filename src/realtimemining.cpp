#include "Python.h"

#include "../real-time-mining/CustomSink.h"
#include "../real-time-mining/Simulation.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;


Simulation sm;


static PyObject *SensorIDError;


PyObject* startSimulation(PyObject *self, PyObject *args)
{
	sm.start();

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* setSensorThreshold(PyObject *self, PyObject *args)
{
	const char *argSensorID;
	double threshold;

	// Parses arguments
	if (!PyArg_ParseTuple(args, "sf", &argSensorID, &threshold))
		return NULL;

	EnvironmentMonitor::SensorID sensorID;

	// Converts string to sensor ID
	if (std::string(argSensorID).compare("CH4") == 0)
		sensorID = EnvironmentMonitor::SensorID::CH4;
	else if (std::string(argSensorID).compare("CO") == 0)
		sensorID = EnvironmentMonitor::SensorID::CO;
	else if (std::string(argSensorID).compare("AIR_FLOW") == 0)
		sensorID = EnvironmentMonitor::SensorID::AIR_FLOW;
	else
	{
		PyErr_SetString(SensorIDError, ("Unrecognized sensor ID " + std::string(argSensorID)).c_str());
		return NULL;
	}

	// C++ function call
	sm.setSensorThreshold(sensorID, threshold);

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* getSensorData(PyObject *self, PyObject *args)
{
	const char *arg;

	// Parses arguments
	if (!PyArg_ParseTuple(args, "s", &arg))
		return NULL;

	EnvironmentMonitor::SensorID sensorID;

	// Converts string to sensor ID
	if (std::string(arg).compare("CH4") == 0)
		sensorID = EnvironmentMonitor::SensorID::CH4;
	else if (std::string(arg).compare("CO") == 0)
		sensorID = EnvironmentMonitor::SensorID::CO;
	else if (std::string(arg).compare("AIR_FLOW") == 0)
		sensorID = EnvironmentMonitor::SensorID::AIR_FLOW;
	else 
	{
		PyErr_SetString(SensorIDError, ("Unrecognized sensor ID " + std::string(arg)).c_str());
		return NULL;
	}

	double value;
	bool alarm;

	// C++ function call
	sm.getSensorData(sensorID, value, alarm);

	// Returns tuple
	PyObject* tuple = PyTuple_New(2);

	PyTuple_SetItem(tuple, 0, PyFloat_FromDouble(value));
	PyTuple_SetItem(tuple, 1, PyBool_FromLong(alarm ? 1 : 0));

	return tuple;
}


PyObject* getPumpAlarm(PyObject *self, PyObject *args)
{
	return PyBool_FromLong(sm.getPumpAlarm() ? 1 : 0);
}


PyObject* isPumpOn(PyObject *self, PyObject *args)
{
	return PyBool_FromLong(sm.isPumpOn() ? 1 : 0);
}


PyObject* turnPumpOn(PyObject *self, PyObject *args)
{
	sm.turnPumpOn();

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* turnPumpOff(PyObject *self, PyObject *args)
{
	sm.turnPumpOff();

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* getWaterLevelAlarms(PyObject *self, PyObject *args)
{
	bool highLevelAlarm;
	bool lowLevelAlarm;

	// C++ function call
	sm.getWaterLevelAlarms(highLevelAlarm, lowLevelAlarm);

	// Returns tuple
	PyObject* tuple = PyTuple_New(2);

	PyTuple_SetItem(tuple, 0, PyBool_FromLong(highLevelAlarm ? 1 : 0));
	PyTuple_SetItem(tuple, 1, PyBool_FromLong(lowLevelAlarm ? 1 : 0));

	return tuple;
}


PyObject* setHighLevelThreshold(PyObject *self, PyObject *args)
{
	double threshold;

	// Parses arguments
	if (!PyArg_ParseTuple(args, "f", &threshold))
		return NULL;

	// C++ function call
	sm.setHighLevelThreshold(threshold);

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* setLowLevelThreshold(PyObject *self, PyObject *args)
{
	double threshold;

	// Parses arguments
	if (!PyArg_ParseTuple(args, "f", &threshold))
		return NULL;

	// C++ function call
	sm.setLowLevelThreshold(threshold);

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* getWaterLevel(PyObject *self, PyObject *args)
{
	return PyFloat_FromDouble(sm.getWaterLevel());
}


PyObject* getLoggingData(PyObject *self, PyObject *args)
{
	CustomSink& instance = CustomSink::getInstance();

	std::vector<std::string> messages;
	instance.getMessages(messages);

	std::string combined;

	for (auto iter = messages.begin(); iter != messages.end(); ++iter)
	{
		// Cuts of time stamp
		std::string message = (*iter).substr((*iter).find(']') + 2);

		// Breaks message if it spans more than one line
		if (message.length() >= 43)
			message = message.insert(message.substr(0, 43).rfind(" ") + 1, "\n");

		combined += message;
	}

	return PyUnicode_FromString(combined.c_str());
}


// Defines the methods of the module
static PyMethodDef realTimeMining[] = {      
	{ "start_simulation", (PyCFunction)startSimulation, METH_NOARGS, "Starts the real-time mining simulation" },
	{ "set_sensor_threshold", (PyCFunction)setSensorThreshold, METH_VARARGS, "Sets the threshold of the specified sensor" },
	{ "get_sensor_data", (PyCFunction)getSensorData, METH_VARARGS, "Gets data of the specified sensor" },
	{ "get_pump_alarm", (PyCFunction)getPumpAlarm, METH_NOARGS, "Returns if the pump alarm is on" },
	{ "is_pump_on", (PyCFunction)isPumpOn, METH_NOARGS, "Returns if the pump is on" },
	{ "turn_pump_on", (PyCFunction)turnPumpOn, METH_NOARGS, "Turns the pump on" },
	{ "turn_pump_off", (PyCFunction)turnPumpOff, METH_NOARGS, "Turns the pump off" },
	{ "get_water_level_alarms", (PyCFunction)getWaterLevelAlarms, METH_NOARGS, "Returns the water level alarms" },
	{ "set_high_level_threshold", (PyCFunction)setHighLevelThreshold, METH_VARARGS, "Sets the high water level threshold" },
	{ "set_low_level_threshold", (PyCFunction)setLowLevelThreshold, METH_VARARGS, "Sets the low water level threshold" },
	{ "get_water_level", (PyCFunction)getWaterLevel, METH_NOARGS, "Returns the water level" },
	{ "get_logging_data", (PyCFunction)getLoggingData, METH_NOARGS, "Returns logging data" },
	{ nullptr, nullptr, 0, nullptr }
};


// Defines the module
static PyModuleDef realTimeMiningModule = {
	PyModuleDef_HEAD_INIT,
	"realtimemining",
	"A module that simulates real-time mining",
	0,
	realTimeMining
};


PyMODINIT_FUNC PyInit_realtimemining() {
	PyObject *module = PyModule_Create(&realTimeMiningModule);

	if (module == NULL)
		return NULL;

	// Adds custom sensor ID exception
	SensorIDError = PyErr_NewException("sensor_id.error", NULL, NULL);
	Py_INCREF(SensorIDError);
	PyModule_AddObject(module, "error", SensorIDError);

	return module;
}

