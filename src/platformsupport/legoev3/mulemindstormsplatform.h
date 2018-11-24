#ifndef MULEMINDSTORMSPLATFORM_H
#define MULEMINDSTORMSPLATFORM_H

#define MULE_MINDSTORMS_MOTOROFF -798
#define MULE_MINDSTORMS_MOTORPIN 5
// I had to implement a fake fifth pin so that you could access the Mindstorms motors the motors somehow
#define MULE_MINDSTORMS_MOTORSPEED 40
#define MULE_MINDSTORMS_PWMFREQUENCY 10000
// MULE_MINDSTORMS_PWMFREQUENCY value taken from http://icube-avr.unistra.fr/en/index.php/Hacking_the_Lego_EV3
#define MULE_MINDSTORMS_PWMMAXRANGE 2500
// taken from https://github.com/joan2937/pigpio/blob/master/pigpio.h for Raspberry Pi compatibility
#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cmath>
#include "3rdparty/EV3-API-master/API/ev3.h"

#define MULE_HOMEDIRECTORY "/home/root/lms2012/timkoisoft/libMule"
#define MULE_DOCUMENTSDIRECTORY "/home/root/lms2012/timkoisoft/libMule"
#define MULE_TEMPDIRECTORY "/tmp"
#define MULE_INPUT 0
#define MULE_OUTPUT 1



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
    std::vector<MuleDevice*> devlist;
    
    void legoSetPinType(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE type);
};

#endif // MULEDUMMYPLATFORM_H
