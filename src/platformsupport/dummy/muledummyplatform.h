#ifndef MULEDUMMYPLATFORM_H
#define MULEDUMMYPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <sys/stat.h>

class MuleDummyPlatform : public MuleCommonPlatform
{
public:
    MuleDummyPlatform();

    bool initialize();
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return "dummy";
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

private:
    std::vector<MuleDevice*> devlist;
};

#endif // MULEDUMMYPLATFORM_H
