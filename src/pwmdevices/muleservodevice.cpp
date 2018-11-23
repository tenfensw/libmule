#include "pwmdevices/muleservodevice.h"

MuleServoDevice::MuleServoDevice(int devpin) : MulePWMDevice(devpin) {
	this->setFrequency(50);
}

bool MuleServoDevice::rotate(int angle) {
	int dcycle = (((angle / 180) + 1) * 5);
	return this->start(dcycle);
}

bool MuleServoDevice::goBackAndForth(int startangle, int endangle, int times, double delay) {
	if (times < 1)
		return false;
	if (this->rotate(startangle) == false)
		return false;
	for (int i = 0; i < times; i++) {
		this->rotate(endangle);
		mulesleep(delay);
		this->rotate(startangle);
		mulesleep(delay);
	}
	return true;
}
