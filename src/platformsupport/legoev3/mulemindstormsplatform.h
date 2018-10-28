#ifndef MULEMINDSTORMSPLATFORM_H
#define MULEMINDSTORMSPLATFORM_H

#define MULE_MINDSTORMS_MOTOROFF -798
#include "platformsupport/common/mulecommonplatform.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cmath>
#include <ev3.h>

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
#endif

private:
    std::vector<MuleDevice*> devlist;
};

#endif // MULEDUMMYPLATFORM_H
