#include "muledevice.h"
#include "muleapplication.h"

MuleDevice::MuleDevice(const MULE_OTHER_HWPINTYPE& pin)
{
    if (dvinit(pin))
        muledebug("MuleDevice with pin " + muleinttostr(pin) + " successfully initialized");
    else
        muleexception(8, "MuleDevice(" + muleinttostr(pin) + ") could not be initialized", true);
}

bool MuleDevice::dvinit(const MULE_OTHER_HWPINTYPE& pin) {
    pinNum = pin;
    if (!(MuleApplicationWideData::appWideFirstInstance))
        return false;
    muledebug("I'm going to access the only running MuleApplication instance to get a pointer to MuleCurrentPlatform class");
    MuleApplication* maInstance = (MuleApplication*)(MuleApplicationWideData::appWideFirstInstance);
    muledebug("Now run getPlatformClass()");
    mcpInstance = (void*)(maInstance->getPlatformClass());
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    convertedMcpInstance->getPinMode(pin);
    return true;
}

MULE_OTHER_HWPINTYPE MuleDevice::read() {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->readFromPin(pinNum);
}

bool MuleDevice::write(const MULE_OTHER_HWPINTYPE& val) {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->writeToPin(pinNum, val);
}

MULE_OTHER_HWPINTYPE MuleDevice::mode() {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->getPinMode(pinNum);
}

bool MuleDevice::setMode(const MULE_OTHER_HWPINTYPE& mode) {
    MuleCurrentPlatform* convertedMcpInstance = (MuleCurrentPlatform*)(mcpInstance);
    return convertedMcpInstance->setPinMode(pinNum, mode);
}

bool MuleDevice::trigger(const MULE_OTHER_HWPINTYPE& pulselen, const MULE_OTHER_HWPINTYPE& level) {
    muleexception(2, "Unimplemented (MuleDevice::trigger)", true);
    return false;
}
