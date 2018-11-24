#include "sensors/mulesensor.h"

MuleSensor::MuleSensor(int devpin) : MuleDevice(devpin) {
	this->setPinType(Sensor);
}

MuleSensor::~MuleSensor() {
}

bool MuleSensor::waitUntilTriggered() {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->sensorWaitUntilTriggered(pinNum);
}
