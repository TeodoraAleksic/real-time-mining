#include "Python.h"
#include "EnvironmentMonitor.h"

#include <string>

using namespace std;


EnvironmentMonitor em;


static PyObject *SensorIDError;


PyObject* startSimulation(PyObject *self, PyObject *args)
{
	em.start();

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* readSensor(PyObject *self, PyObject *args)
{
	const char *arg;

	if (!PyArg_ParseTuple(args, "s", &arg))
		return NULL;

	EnvironmentMonitor::SensorID sensorID;

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
	bool error;
	bool alarm;

	em.getSensorData(sensorID, value, error, alarm);

	PyObject* tuple = PyTuple_New(3);

	PyTuple_SetItem(tuple, 0, PyFloat_FromDouble(value));
	PyTuple_SetItem(tuple, 1, PyBool_FromLong(error ? 1 : 0));
	PyTuple_SetItem(tuple, 2, PyBool_FromLong(alarm ? 1 : 0));

	return tuple;
}


// Defines the method of the module
static PyMethodDef realTimeMining[] = {      
	{ "start_simulation", (PyCFunction)startSimulation, METH_NOARGS, "Starts the real-time mining simulation" },
	{ "read_sensor", (PyCFunction)readSensor, METH_VARARGS, "Reads current value of specified sensor" },
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

