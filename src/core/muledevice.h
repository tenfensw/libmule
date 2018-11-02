#ifndef MULEDEVICE_H
#define MULEDEVICE_H

#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"

class MuleDevice
{
public:
    MuleDevice(MULE_OTHER_HWPINTYPE pin);
    ~MuleDevice();
    MULE_OTHER_HWPINTYPE pin() {
        return pinNum;
    }
    MULE_OTHER_HWPINTYPE read();
    bool write(MULE_OTHER_HWPINTYPE val);
    MULE_OTHER_HWPINTYPE mode();
    bool setMode(MULE_OTHER_HWPINTYPE mode);
    bool trigger(MULE_OTHER_HWPINTYPE pulselen, MULE_OTHER_HWPINTYPE level = 0);

private:
    bool dvinit(MULE_OTHER_HWPINTYPE pin);

    void* mcpInstance;
    int pinNum;
};

#endif // MULEDEVICE_H
