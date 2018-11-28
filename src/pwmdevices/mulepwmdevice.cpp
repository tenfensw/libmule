//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

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
