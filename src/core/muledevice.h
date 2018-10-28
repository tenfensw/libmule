#ifndef MULEDEVICE_H
#define MULEDEVICE_H

#include "core/muleglobalfunctions.h"

class MuleDevice
{
public:
    MuleDevice(const MULE_OTHER_HWPINTYPE& pin);
    ~MuleDevice();
    MULE_OTHER_HWPINTYPE pin() {
        return pinNum;
    }
    MULE_OTHER_HWPINTYPE read();
    bool write(const MULE_OTHER_HWPINTYPE& val);
    MULE_OTHER_HWPINTYPE mode();
    bool setMode(const MULE_OTHER_HWPINTYPE& mode);
    bool trigger(const MULE_OTHER_HWPINTYPE& pulselen, const MULE_OTHER_HWPINTYPE& level = 0);

private:
    bool dvinit(const MULE_OTHER_HWPINTYPE& pin);

    void* mcpInstance;
    int pinNum;
};

#endif // MULEDEVICE_H
