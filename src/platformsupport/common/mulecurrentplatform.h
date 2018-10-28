#ifndef MULECURRENTPLATFORM_H
#define MULECURRENTPLATFORM_H

#include <string>
#include <vector>
#include "core/muleglobalfunctions.h"
#include "core/muledevice.h"
#ifdef MULE_PLATFORM_DUMMY
#include "platformsupport/dummy/muledummyplatform.h"
#define MULE_INTERNAL_CURRENTPLATFORMCLASS MuleDummyPlatform
#define MULE_INTERNAL_CURRENTPLATFORMNAME std::string("dummy")
#elif defined(MULE_PLATFORM_LEGOEV3)
#include "platformsupport/legoev3/mulemindstormsplatform.h"
#define MULE_INTERNAL_CURRENTPLATFORMCLASS MuleMindstormsPlatform
#define MULE_INTERNAL_CURRENTPLATFORMNAME std::string("legoev3")
#else
#error No supported platforms were detected
#endif

class MuleCurrentPlatform
{
public:
    MuleCurrentPlatform();
    std::vector<MuleDevice*> getDevices() {
        return platformClass->getDevices();
    }
    MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin) {
        return platformClass->getPinMode(pin);
    }
    bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
        return platformClass->setPinMode(pin, mode);
    }
    MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin) {
        return platformClass->readFromPin(pin);
    }
    bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
        return platformClass->writeToPin(pin, ct);
    }
    MULE_OTHER_STRINGTYPE readFromFile(MULE_OTHER_STRINGTYPE file) {
        return platformClass->readFromFile(file);
    }
    bool writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
        return platformClass->writeToFile(file, ct);
    }
    bool fileExists(MULE_OTHER_STRINGTYPE file) {
        return platformClass->fileExists(file);
    }
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return platformClass->getPlatformName();
    }

private:
    MULE_INTERNAL_CURRENTPLATFORMCLASS* platformClass;
};

#endif // MULECURRENTPLATFORM_H
