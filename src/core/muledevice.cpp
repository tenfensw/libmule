#include "core/muledevice.h"
#include "core/muleapplication.h"

MuleDevice::MuleDevice(MULE_OTHER_HWPINTYPE pin)
{
    if (dvinit(pin))
        muledebug("MuleDevice with pin " + muleinttostr(pin) + " successfully initialized");
    else
        muleexception(8, "MuleDevice(" + muleinttostr(pin) + ") could not be initialized", true);
}

bool MuleDevice::dvinit(MULE_OTHER_HWPINTYPE pin) {
    pinNum = pin;
    if (!(MuleApplicationWideData::appWideFirstInstance))
        return false;
    muledebug("I'm going to access the only running MuleApplication instance to get a pointer to MuleCurrentPlatform class");
    MuleApplication* maInstance = (MuleApplication*)(MuleApplicationWideData::appWideFirstInstance);
    muledebug("Now run getPlatformClass()");
    if (maInstance->areNecessaryPartsReady == true) {
	muledebug("maInstance->areNecessaryPartsReady == true");
    	mcpInstance = (void*)(maInstance->getPlatformClass());
    	MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    	convertedMcpInstance->getPinMode(pin);
	this->setPinType(Generic);
    }
    else
	muledebug("maInstance->areNecessaryPartsReady != true");
    return true;
}

MULE_OTHER_HWPINTYPE MuleDevice::read() {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->readFromPin(pinNum);
}

bool MuleDevice::write(MULE_OTHER_HWPINTYPE val) {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->writeToPin(pinNum, val);
}

MULE_OTHER_HWPINTYPE MuleDevice::mode() {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->getPinMode(pinNum);
}

bool MuleDevice::setMode(MULE_OTHER_HWPINTYPE mode) {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->setPinMode(pinNum, mode);
}

bool MuleDevice::trigger(MULE_OTHER_HWPINTYPE pulselen, MULE_OTHER_HWPINTYPE level) {
    muledebug("Software implementation of GPIO trigger is active");
    try {
    	this->write(level);
    	mulemicrosecsleep(pulselen);
    	if (level != 0)
    		this->write(0);
    	else
    		this->write(1);
    	return true;
    }
    catch (...) {
    	muledebug("ASSERT or SEGFAULT was caused, will return false");
    }
    return false;
}

bool MuleDevice::setPullUpDown(MULE_OTHER_HWPINTYPE val) {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
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

