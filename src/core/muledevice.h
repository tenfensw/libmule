#ifndef MULEDEVICE_H
#define MULEDEVICE_H

#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"

class MuleDevice
{
public:
    const static int Generic = 0;
    const static int PWM = 1;
    const static int Sensor = 2;

    MuleDevice(MULE_OTHER_HWPINTYPE pin);
    ~MuleDevice();
    MULE_OTHER_HWPINTYPE pin() {
        return pinNum;
    }
    MULE_OTHER_HWPINTYPE getPin() {
	return pin();
    }
    MULE_OTHER_HWPINTYPE read();
    bool write(MULE_OTHER_HWPINTYPE val);
    MULE_OTHER_HWPINTYPE mode();
    MULE_OTHER_HWPINTYPE getMode() {
	return mode();
    }
    bool setMode(MULE_OTHER_HWPINTYPE mode);
    bool trigger(MULE_OTHER_HWPINTYPE pulselen, MULE_OTHER_HWPINTYPE level = 0);
    int pinType() {
	return pType;
    }
    int getPinType() {
	return pinType();
    }
    bool setPinType(int type);
    bool setPullUpDown(MULE_OTHER_HWPINTYPE val);
    bool noPullUpDown();
    bool resetPullUpDown() { return this->noPullUpDown(); }

protected:
    bool dvinit(MULE_OTHER_HWPINTYPE pin);

    void* mcpInstance;
    int pinNum;
    int pType;
};

#endif // MULEDEVICE_H
