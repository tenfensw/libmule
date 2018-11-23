#include "sensors/mulelightsensor.h"

MuleLightSensor::MuleLightSensor(int devpin) : MuleDevice(devpin) {
	this->setPinType(Sensor);
}

bool MuleLightSensor::waitUntilTriggered() {
	// first set it to low
	if ((this->setMode(MULE_OUTPUT) == false) || (this->write(0) == false))
		return false;
	mulesleep(0.1);
	// turn it back into an input pin
	if (this->setMode(MULE_INPUT) == false)
		return false;
	
	int numberofchecks = 0;
	while (this->read() == 0)
		numberofchecks = numberofchecks + 1;
	
	return true;
}
