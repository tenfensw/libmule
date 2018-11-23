
#ifndef MULECOMMONPLATFORM_H
#define MULECOMMONPLATFORM_H

#include "core/muledevice.h"
#include "core/muleconfig.h"
#include <vector>
#include <string>
#include <cstdlib>
#include "core/muleglobalfunctions.h"

class MuleCommonPlatform
{
public:
    MuleCommonPlatform() {}

    virtual bool initialize() {}
#ifdef MULE_FEATURES_CORE
    virtual std::vector<MuleDevice*> getDevices() {}
    virtual MULE_OTHER_HWPINTYPE getPinMode(MULE_OTHER_HWPINTYPE pin) {}
    virtual bool setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {}
    virtual MULE_OTHER_HWPINTYPE readFromPin(MULE_OTHER_HWPINTYPE pin) {}
    virtual bool writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {}
    virtual bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {}
#endif
#ifdef MULE_FEATURES_FILEIO
    virtual MULE_OTHER_STRINGTYPE readFromFile(MULE_OTHER_STRINGTYPE file) {}
    virtual bool writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {}
    virtual bool fileExists(MULE_OTHER_STRINGTYPE file) {}
    virtual bool deleteFile(MULE_OTHER_STRINGTYPE file) {}
#endif
#ifdef MULE_FEATURES_SOUND
    virtual void doBeep() {}
    virtual bool playWaveFile(MULE_OTHER_STRINGTYPE filename) {}
    virtual MULE_OTHER_STRINGTYPE getSoundBackend() {}
    virtual bool stopAllSounds() {}
#endif
#ifdef MULE_FEATURES_PWMDEVICES
    virtual bool startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) {}
    virtual MULE_OTHER_HWPINTYPE getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) {}
    virtual MULE_OTHER_HWPINTYPE getPWMRange(MULE_OTHER_HWPINTYPE pin) {}
    virtual bool setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) {}
    virtual MULE_OTHER_HWPINTYPE getPWMFrequency(MULE_OTHER_HWPINTYPE pin) {}
    virtual bool setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) {}
#endif
#ifdef MULE_FEATURES_SENSORS
    virtual bool photoresistorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) {}
    virtual bool buttonWaitUntilPressed(MULE_OTHER_HWPINTYPE pin) {}
#endif


protected:
    void platformInitializationException(int erc, MULE_OTHER_STRINGTYPE message) {
        muleprintf("libMule " + muleinttostr(MULE_VERSION_MAJOR) + "." + muleinttostr(MULE_VERSION_MINOR) + "." + muleinttostr(MULE_VERSION_UPDATE) + " Platform Initialization Error\n");
        muleprintf( "A critical error occured during the initialization of the device platform.\n");
        muleprintf("\n");
        muleprintf("Error №" + muleinttostr(erc) + ": " + message);
        muleprintf("\n\n");
        muleprintf("The application will now exit now.\n");
        int exitcode = 400 + erc;
	std::exit(exitcode);
    }


};

#endif // MULECOMMONPLATFORM_H
