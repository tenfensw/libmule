#ifndef MULEARDUINOAVRPLATFORM_H
#define MULEARDUINOAVRPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include "ArduinoSTL.h"

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


class MuleArduinoAVRPlatform : public MuleCommonPlatform {
    public:
      MuleArduinoAVRPlatform();
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
};

#endif
