//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2019 Tim K <timprogrammer@rambler.ru>
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

#ifndef MULELINUXGPIOPLATFORM_H
#define MULELINUXGPIOPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>

#define MULE_HOMEDIRECTORY MULE_OTHER_STRINGTYPE(getenv("HOME"))
#define MULE_DOCUMENTSDIRECTORY MULE_HOMEDIRECTORY + "/Documents"
#define MULE_TEMPDIRECTORY MULE_OTHER_STRINGTYPE("/tmp")
#define MULE_INPUT 0
#define MULE_OUTPUT 1
#define MULE_LOW 0
#define MULE_HIGH 1
#define MULE_PUD_OFF 0
#define MULE_PUD_DOWN 1
#define MULE_PUD_UP 2


struct MuleLinuxGPIOPin {
	int num; // pin number
	MULE_OTHER_STRINGTYPE path; // direct path to simulator socket related to this pin
	bool input; // is input pin
	int val; // digital value
};

class MuleLinuxGPIOPlatform : public MuleCommonPlatform
{
public:
    MuleLinuxGPIOPlatform();
    ~MuleLinuxGPIOPlatform();

    bool initialize();
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return "linuxgpio";
    }
#ifdef MULE_FEATURES_CORE
    std::vector<MuleDevice*> getDevices();
    MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin);
    bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode);
    MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin);
    bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct);
    bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val);
#endif
#ifdef MULE_FEATURES_PWMDEVICES
    virtual bool startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) { return false; }
    virtual MULE_OTHER_HWPINTYPE getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) { return -1; }
    virtual MULE_OTHER_HWPINTYPE getPWMRange(MULE_OTHER_HWPINTYPE pin) { return -1; }
    virtual bool setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) { return false; }
    virtual MULE_OTHER_HWPINTYPE getPWMFrequency(MULE_OTHER_HWPINTYPE pin) { return -1; }
    virtual bool setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) { return false; }
#endif
#ifdef MULE_FEATURES_SENSORS
    bool sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin);
#endif
#ifdef MULE_FEATURES_FILEIO
    MULE_OTHER_STRINGTYPE readFromFile(MULE_OTHER_STRINGTYPE file);
    bool writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct);
    bool fileExists(MULE_OTHER_STRINGTYPE file);
    bool deleteFile(MULE_OTHER_STRINGTYPE file);
#endif
#ifdef MULE_FEATURES_SOUND
    void doBeep();
    bool playWaveFile(MULE_OTHER_STRINGTYPE filename);
    MULE_OTHER_STRINGTYPE getSoundBackend() {
	return "dynamic";
    }
    bool stopAllSounds();
#endif

private:
    void internal_cleanDevList();
    MULE_OTHER_STRINGTYPE internal_readToString(MULE_OTHER_STRINGTYPE fn);
    MULE_OTHER_STRINGTYPE internal_muleIntToStr(int in) { return muleinttostr(in); }
    bool internal_fileExists(MULE_OTHER_STRINGTYPE fn);
    bool internal_writeToFile(MULE_OTHER_STRINGTYPE fn, MULE_OTHER_STRINGTYPE ct);
    MuleLinuxGPIOPin internal_readPin(int pn);
    bool internal_flushPin(const MuleLinuxGPIOPin& pin);
    int internal_stringToNum(MULE_OTHER_STRINGTYPE in);
    bool internal_dirExists(MULE_OTHER_STRINGTYPE dir);
};

#endif // MULEMICROCONTROLLERSIMPLATFORM_H
