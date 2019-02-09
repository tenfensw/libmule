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

#include "core/muledevice.h"
#include "core/muleapplication.h"

MuleDevice::MuleDevice(MULE_OTHER_HWPINTYPE pin)
{
    if (dvinit(pin))
        muledebug("MuleDevice with pin " + muleinttostr(pin) + " successfully initialized");
    else if (pin != -1)
        muleexception(8, "MuleDevice(" + muleinttostr(pin) + ") could not be initialized", true);
}

MuleDevice::~MuleDevice() {
}

bool MuleDevice::dvinit(MULE_OTHER_HWPINTYPE pin) {
    pinNum = pin;
    muledebug("pinNum = " + muleinttostr(pinNum));
    if (!(MuleApplication::getRunningInstance()) || pin == -1)
        return false;
    muledebug("I'm going to access the only running MuleApplication instance to get a pointer to MuleCommonPlatform class");
    MuleApplication* maInstance = (MuleApplication::getRunningInstance());
    muledebug("Now run getPlatformClass()");
    if (maInstance->areNecessaryPartsReady == true) {
	muledebug("maInstance->areNecessaryPartsReady == true");
    	convertedMcpInstance = (MULE_INTERNAL_CURRENTPLATFORMCLASS*)(maInstance->getPlatformClass());
    	convertedMcpInstance->getPinMode(pin);
	setPinType(Generic);
    }
    else
	muledebug("maInstance->areNecessaryPartsReady != true");
    return true;
}

MULE_OTHER_HWPINTYPE MuleDevice::read() {
    
    return convertedMcpInstance->readFromPin(pinNum);
}

bool MuleDevice::write(MULE_OTHER_HWPINTYPE val) {
    
    return convertedMcpInstance->writeToPin(pinNum, val);
}

MULE_OTHER_HWPINTYPE MuleDevice::mode() {
    
    return convertedMcpInstance->getPinMode(pinNum);
}

bool MuleDevice::setMode(MULE_OTHER_HWPINTYPE mode) {
    
    return convertedMcpInstance->setPinMode(pinNum, mode);
}

bool MuleDevice::continuousWrite(int times, double delay) {
    muledebug("times = " + muleinttostr(times));
    muledebug("(int)(delay) = " + muleinttostr((int)(delay)));
    int startval = read();
    if (startval == MULE_LOW)
	startval = MULE_HIGH;
    else
	startval = MULE_LOW;
    muledebug("startval = " + muleinttostr(startval));
    for (int i = 0; i < times; i++) {
	    muledebug("lap " + muleinttostr(i));
	    mulesleep(delay);
	    if (write(startval))
		    return false;
	    if (startval == MULE_LOW)
		startval = MULE_HIGH;
	    else
		startval = MULE_LOW;
	    muledebug("startval = " + muleinttostr(startval));
    }
    return true;
}

bool MuleDevice::trigger(MULE_OTHER_HWPINTYPE pulselen, MULE_OTHER_HWPINTYPE level) {
    muledebug("Software implementation of GPIO trigger is active");
    write(level);
    mulemicrosecsleep(pulselen);
    if (level != 0)
	write(0);
    else
	write(1);
    return true;
}

bool MuleDevice::setPullUpDown(MULE_OTHER_HWPINTYPE val) {
    return convertedMcpInstance->setPullUpDown(pinNum, val);
}

bool MuleDevice::setPinType(int type) {
	if (type < 0 || type > 2)
		return false;
	pType = type;
	if (type == Sensor)
		setMode(MULE_INPUT);
	return true;
}

