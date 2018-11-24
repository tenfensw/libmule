#ifndef MULEPIGPIOPLATFORM_H
#define MULEPIGPIOPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <sys/stat.h>
#include <pigpio.h>

#define MULE_HOMEDIRECTORY "/home/" + MULE_OTHER_STRINGTYPE(getenv("USER"))
#define MULE_DOCUMENTSDIRECTORY MULE_HOMEDIRECTORY + "/Documents"
#define MULE_TEMPDIRECTORY "/tmp"
#define MULE_INPUT	0
#define MULE_OUTPUT	1
#define MULE_LOW	0
#define MULE_HIGH	1
#define MULE_PUD_OFF	0
#define MULE_PUD_DOWN	1
#define MULE_PUD_UP	2

class MulePigpioPlatform : public MuleCommonPlatform
{
public:
    MulePigpioPlatform();
    ~MulePigpioPlatform();

    bool initialize();
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return "pigpio";
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
	return "dynamic";
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
    std::vector<MuleDevice*> devlist;
};

#endif // MulePigpioPlatform_H
