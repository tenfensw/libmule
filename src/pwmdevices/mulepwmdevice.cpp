#include "pwmdevices/mulepwmdevice.h"

MulePWMDevice::MulePWMDevice(int devpin) : MuleDevice(devpin) {
	this->setPinType(PWM);
	deviceIsTurnedOn = false;
}

MulePWMDevice::~MulePWMDevice() {
}

bool MulePWMDevice::start(MULE_OTHER_HWPINTYPE dutycycle) {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	bool result = convertedMcpInstance->startPWM(pinNum, dutycycle);
	if (result == true && dutycycle != 0)
		deviceIsTurnedOn = true;
	else
		deviceIsTurnedOn = false;
	return result;
}

int MulePWMDevice::getRange() {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->getPWMRange(pinNum);
}

int MulePWMDevice::getDutyCycle() {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->getPWMDutyCycle(pinNum);
}

int MulePWMDevice::getFrequency() {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->getPWMFrequency(pinNum);
}

bool MulePWMDevice::isOn() {
	return deviceIsTurnedOn;
}

bool MulePWMDevice::setRange(MULE_OTHER_HWPINTYPE range) {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->setPWMRange(pinNum, range);
}

bool MulePWMDevice::setFrequency(MULE_OTHER_HWPINTYPE frequency) {
	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
	return convertedMcpInstance->setPWMFrequency(pinNum, frequency);
}
