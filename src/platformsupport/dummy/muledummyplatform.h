#ifndef MULEDUMMYPLATFORM_H
#define MULEDUMMYPLATFORM_H

#include "platformsupport/common/mulecommonplatform.h"
#include "core/muleconfig.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <sys/stat.h>

#ifdef MULE_OS_UNIX
#define MULE_HOMEDIRECTORY "/home/" + MULE_OTHER_STRINGTYPE(getenv("USER"))
#define MULE_DOCUMENTSDIRECTORY MULE_HOMEDIRECTORY + "/Documents"
#else
#define MULE_HOMEDIRECTORY "/undefined/path"
#define MULE_DOCUMENTSDIRECTORY "/undefined/path"
#endif
#define MULE_TEMPDIRECTORY "/tmp"
#define MULE_INPUT  0
#define MULE_OUTPUT 1


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
#ifdef MULE_FEATURES_SOUND
    void doBeep();
    bool playWaveFile(MULE_OTHER_STRINGTYPE filename);
    MULE_OTHER_STRINGTYPE getSoundBackend() {
	return "dynamic";
    }
    bool stopAllSounds();
#endif

private:
    std::vector<MuleDevice*> devlist;
};

#endif // MULEDUMMYPLATFORM_H
