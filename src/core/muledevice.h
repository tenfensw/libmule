//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018-2019 Tim K <timprogrammer@rambler.ru>
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

#ifndef MULEDEVICE_H
#define MULEDEVICE_H

#include "core/muleconfig.h"
#include "core/muleglobalfunctions.h"
#include "platformsupport/common/mulecommonplatform.h"

class MuleCommonPlatform;

#ifndef MULE_HIGH
#define MULE_HIGH 1
#endif
#ifndef MULE_LOW
#define MULE_LOW 0
#endif

typedef enum { Generic = 0, PWM = 1, Sensor = 2 } MuleDeviceType;

class MuleDevice
{
public:
    MuleDevice(MULE_OTHER_HWPINTYPE pin);
    ~MuleDevice();
    MULE_OTHER_HWPINTYPE pin() {
        muledebug("return " + muleinttostr(pinNum));
        return pinNum;
    }
    MULE_OTHER_HWPINTYPE getPin() {
	return pin();
    }
    MULE_OTHER_HWPINTYPE read();
    bool write(MULE_OTHER_HWPINTYPE val);
    virtual bool on() { return write(MULE_HIGH); }
    virtual bool off() { return write(MULE_LOW); }
    MULE_OTHER_HWPINTYPE mode();
    MULE_OTHER_HWPINTYPE getMode() {
	return mode();
    }
    bool setMode(MULE_OTHER_HWPINTYPE mode);
    bool trigger(MULE_OTHER_HWPINTYPE pulselen, MULE_OTHER_HWPINTYPE level = 0);
    MuleDeviceType pinType() {
	return pType;
    }
    MuleDeviceType getPinType() {
	return pinType();
    }
    bool setPullUpDown(MULE_OTHER_HWPINTYPE val);
    bool continuousWrite(int times = 2, double delay = 0.5);

protected:
    bool dvinit(MULE_OTHER_HWPINTYPE pin);

    bool setPinType(MuleDeviceType type);
    MuleCommonPlatform* convertedMcpInstance;
    int pinNum;
    int pType;
};

#endif // MULEDEVICE_H
