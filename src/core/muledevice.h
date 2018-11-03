#ifndef MULEDEVICE_H
#define MULEDEVICE_H

#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"

class MuleDevice
{
public:
    const int Other = 0;
#ifdef MULE_FEATURES_ADDITIONALDEVICES
    const int Motor = 1;
    const int Lamp = 2;
    const int Button = 3;
    const int Screen = 4;
    const int TemperatureSensor = 5;
#endif

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

private:
    bool dvinit(MULE_OTHER_HWPINTYPE pin);
    int dvprobepin();

    void* mcpInstance;
    int pinNum;
    int pType;
};

#endif // MULEDEVICE_H
