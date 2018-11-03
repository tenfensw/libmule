#ifndef MULEMINDSTORMSPLATFORM_H
#define MULEMINDSTORMSPLATFORM_H

#define MULE_MINDSTORMS_MOTOROFF -798
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
#ifdef MULE_FEATURES_SENSORS
    std::vector<MuleDevice*> getDevices();
    MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin);
    bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode);
    MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin);
    bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct);
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

private:
    std::vector<MuleDevice*> devlist;
};

#endif // MULEDUMMYPLATFORM_H
