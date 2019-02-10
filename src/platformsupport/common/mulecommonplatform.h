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

#ifndef MULECOMMONPLATFORM_H
#define MULECOMMONPLATFORM_H

#include "core/muledevice.h"
#include "core/muleconfig.h"
#include <vector>
#include <string>
#include <cstdlib>
#include "core/muleglobalfunctions.h"

class MuleDevice;

class MuleCommonPlatform
{
    
public:
    virtual ~MuleCommonPlatform();

    virtual bool initialize() = 0;
    virtual MULE_OTHER_STRINGTYPE getPlatformName();
#ifdef MULE_FEATURES_CORE
    virtual std::vector<MuleDevice*> getDevices();
    virtual MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin) = 0;
    virtual bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) = 0;
    virtual MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin) = 0;
    virtual bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) = 0;
    virtual bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) = 0;
#endif
#ifdef MULE_FEATURES_FILEIO
    virtual MULE_OTHER_STRINGTYPE readFromFile(MULE_OTHER_STRINGTYPE file) = 0;
    virtual bool writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) = 0;
    virtual bool fileExists(MULE_OTHER_STRINGTYPE file) = 0;
    virtual bool deleteFile(MULE_OTHER_STRINGTYPE file) = 0;
#endif
#ifdef MULE_FEATURES_SOUND
    virtual void doBeep() {}
    virtual bool playWaveFile(MULE_OTHER_STRINGTYPE filename) = 0;
    virtual MULE_OTHER_STRINGTYPE getSoundBackend() = 0;
    virtual bool stopAllSounds() = 0;
#endif
#ifdef MULE_FEATURES_PWMDEVICES
    virtual bool startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) = 0;
    virtual MULE_OTHER_HWPINTYPE getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) = 0;
    virtual MULE_OTHER_HWPINTYPE getPWMRange(MULE_OTHER_HWPINTYPE pin) = 0;
    virtual bool setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) = 0;
    virtual MULE_OTHER_HWPINTYPE getPWMFrequency(MULE_OTHER_HWPINTYPE pin) = 0;
    virtual bool setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) = 0;
#endif
#ifdef MULE_FEATURES_SENSORS
    virtual bool sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) = 0;
#endif


protected:
    MuleCommonPlatform() {}
    virtual void internal_cleanDevList();
    void platformInitializationException(int erc, MULE_OTHER_STRINGTYPE message);
    
    std::vector<MuleDevice*> devlist;

};

#endif // MULECOMMONPLATFORM_H
