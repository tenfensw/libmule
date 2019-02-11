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

#ifndef MULEARDUINOAVRPLATFORM_H
#define MULEARDUINOAVRPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include "Arduino.h"

#define MULE_INTERNAL_NOEXCEPTIONS
#define MULE_HOMEDIRECTORY ""
#define MULE_DOCUMENTSDIRECTORY ""
#define MULE_TEMPDIRECTORY ""
#define MULE_INPUT 0x0
#define MULE_OUTPUT 0x1
#define MULE_LOW 0
#define MULE_HIGH 1
#define MULE_PUD_OFF 0x0
#define MULE_PUD_DOWN 0x3
#define MULE_PUD_UP 0x2

#define MULE_ARDUINO_DIGITALPINCOUNT 14
#define MULE_ARDUINO_ANALOGPINCOUNT 6
#define MULE_ARDUINO_LASTDIGITALPIN 13
#define MULE_ARDUINO_MINDUTYCYCLE 0
#define MULE_ARDUINO_MAXDUTYCYCLE 255
#define MULE_ARDUINO_CONSTPWMFREQUENCY 490

class MuleArduinoAVRPlatform : public MuleCommonPlatform {
    public:
      MuleArduinoAVRPlatform();

      bool initialize() { return true; }
      MULE_OTHER_STRINGTYPE getPlatformName() { return "arduinoavr"; }
# ifdef MULE_FEATURES_CORE
      std::vector<MuleDevice*> getDevices();
      MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin);
      bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode);
      MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin);
      bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct);
      bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val);
# endif
# ifdef MULE_FEATURES_PWMDEVICES
      bool startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle);
      MULE_OTHER_HWPINTYPE getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin);
      MULE_OTHER_HWPINTYPE getPWMRange(MULE_OTHER_HWPINTYPE pin);
      bool setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range);
      MULE_OTHER_HWPINTYPE getPWMFrequency(MULE_OTHER_HWPINTYPE pin);
      bool setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq);
# endif
# ifdef MULE_FEATURES_SENSORS
      bool sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin);
# endif

    private:
      bool arduinoIsDigitalPin(MULE_OTHER_HWPINTYPE pin);
      MULE_OTHER_HWPINTYPE arduinoMulePinToNativeAnalog(MULE_OTHER_HWPINTYPE pin);
};

#endif
