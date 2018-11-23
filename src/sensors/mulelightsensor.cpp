#include "sensors/mulelightsensor.h"

MuleLightSensor::MuleLightSensor(int devpin) : MuleDevice(devpin) {
	this->setPinType(Sensor);
}

MuleLightSensor::~MuleLightSensor() {
}

bool MuleLightSensor::waitUntilTriggered() {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->photoresistorWaitUntilTriggered(pinNum);
}
