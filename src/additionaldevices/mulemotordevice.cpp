#include "additionaldevices/mulemotordevice.h"

MuleMotorDevice::MuleMotorDevice(int devpin) : MuleDevice(devpin) {
	this->setPinType(Motor);
	muledebug("Initialized MuleMotorDevice with pin " + muleinttostr(devpin));
}

bool MuleMotorDevice::goBack(int speed) {
	return this->goForward(0 - speed);
}

bool MuleMotorDevice::goForward(int speed) {
	return this->write(speed);
}

void MuleMotorDevice::stop() {
	this->goForward(MULE_OTHER_MOTORSTOPCODE);
}
