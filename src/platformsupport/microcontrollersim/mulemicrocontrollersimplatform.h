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

#ifndef MULEMICROCONTROLLERSIMPLATFORM_H
#define MULEMICROCONTROLLERSIMPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <sys/stat.h>

#ifdef _WIN32
#define MULE_SIMDIRECTORY MULE_OTHER_STRINGTYPE(getenv("TEMP"))
#else
#define MULE_SIMDIRECTORY "/tmp/mulemicrosim"
#endif
#define MULE_HOMEDIRECTORY MULE_SIMDIRECTORY
#define MULE_DOCUMENTSDIRECTORY MULE_SIMDIRECTORY
#define MULE_TEMPDIRECTORY MULE_SIMDIRECTORY
#define MULE_INPUT 0
#define MULE_OUTPUT 1
#define MULE_LOW 0
#define MULE_HIGH 1
#define MULE_PUD_OFF 0
#define MULE_PUD_DOWN 1
#define MULE_PUD_UP 2


struct MuleMicrocontrollerSimulatorPin {
	int num; // pin number
	MULE_OTHER_STRINGTYPE path; // direct path to simulator socket related to this pin
	bool digital; // is digital
	bool input; // is input pin
	int pud; // pull-up-down
	int val; // digital value
	int dc; // duty cycle (analog value)
	int maxdc; // maximum duty cycle
	bool pwm; // is PWM capable or not
	int freq; // frequency
};

class MuleMicrocontrollerSimulatorPlatform : public MuleCommonPlatform
{
public:
    MuleMicrocontrollerSimulatorPlatform();

    bool initialize();
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return "microcontrollersim";
    }
#ifdef MULE_FEATURES_CORE
    MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin);
    bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode);
    MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin);
    bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct);
    bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val);
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
    MULE_OTHER_STRINGTYPE internal_readToString(MULE_OTHER_STRINGTYPE fn);
    MULE_OTHER_STRINGTYPE internal_muleIntToStr(int in);
    bool internal_fileExists(MULE_OTHER_STRINGTYPE fn);
    bool internal_writeToFile(MULE_OTHER_STRINGTYPE fn, MULE_OTHER_STRINGTYPE ct);
    MuleMicrocontrollerSimulatorPin internal_readPin(int pn);
    bool internal_flushPin(const MuleMicrocontrollerSimulatorPin& pin);
};

#endif // MULEMICROCONTROLLERSIMPLATFORM_H
