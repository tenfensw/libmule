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

#ifndef MULEMINDSTORMSPLATFORM_H
#define MULEMINDSTORMSPLATFORM_H

#define MULE_MINDSTORMS_MOTOROFF -798
#define MULE_MINDSTORMS_MOTORPIN 4
// I had to implement a fake forth pin so that you could access the Mindstorms motors the motors somehow
#define MULE_MINDSTORMS_MOTORSPEED 20
#define MULE_MINDSTORMS_PWMFREQUENCY 10000
// MULE_MINDSTORMS_PWMFREQUENCY value taken from http://icube-avr.unistra.fr/en/index.php/Hacking_the_Lego_EV3
#define MULE_MINDSTORMS_PWMMAXRANGE 255
// taken from https://github.com/joan2937/pigpio/blob/master/pigpio.h for Raspberry Pi compatibility
#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cmath>
#include <unistd.h>
#include "3rdparty/EV3-API-master/API/ev3.h"

#define MULE_HOMEDIRECTORY "/home/root/lms2012/timkoisoft/libMule"
#define MULE_DOCUMENTSDIRECTORY "/home/root/lms2012/timkoisoft/libMule"
#define MULE_TEMPDIRECTORY "/tmp"
#define MULE_INPUT 0
#define MULE_OUTPUT 1
#define MULE_LOW 0
#define MULE_HIGH 1
#define MULE_PUD_OFF 0
#define MULE_PUD_DOWN 1
#define MULE_PUD_UP 2

class MuleMindstormsPlatform : public MuleCommonPlatform
{
public:
    MuleMindstormsPlatform();
    ~MuleMindstormsPlatform();

    bool initialize();
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return "legoev3";
    }
#ifdef MULE_FEATURES_CORE
    std::vector<MuleDevice*> getDevices();
    MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin);
    bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode);
    MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin);
    bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct);
    bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val);
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
	return "lego";
    }
    bool stopAllSounds();
#endif
#ifdef MULE_FEATURES_PWMDEVICES
    bool startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle);
    MULE_OTHER_HWPINTYPE getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin);
    MULE_OTHER_HWPINTYPE getPWMRange(MULE_OTHER_HWPINTYPE pin);
    bool setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range);
    MULE_OTHER_HWPINTYPE getPWMFrequency(MULE_OTHER_HWPINTYPE pin);
    bool setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq);
#endif
#ifdef MULE_FEATURES_SENSORS
    bool sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin);
#endif

private:
    void internal_cleanDevList();
    void legoSetPinType(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE type);
};

#endif // MULEDUMMYPLATFORM_H
