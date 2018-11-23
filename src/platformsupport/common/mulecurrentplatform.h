#ifndef MULECURRENTPLATFORM_H
#define MULECURRENTPLATFORM_H

#include <string>
#include <vector>
#include "core/muleconfig.h"
#include "core/muleglobalfunctions.h"
#include "core/muledevice.h"
#ifdef MULE_PLATFORM_DUMMY
#include "platformsupport/dummy/muledummyplatform.h"
#define MULE_INTERNAL_CURRENTPLATFORMCLASS MuleDummyPlatform
#define MULE_INTERNAL_CURRENTPLATFORMNAME MULE_OTHER_STRINGTYPE("dummy")
#elif defined(MULE_PLATFORM_LEGOEV3)
#include "platformsupport/legoev3/mulemindstormsplatform.h"
#define MULE_INTERNAL_CURRENTPLATFORMCLASS MuleMindstormsPlatform
#define MULE_INTERNAL_CURRENTPLATFORMNAME MULE_OTHER_STRINGTYPE("legoev3")
#elif defined(MULE_PLATFORM_RPI)
#include "platformsupport/rpi/muleraspberrypiplatform.h"
#define MULE_INTERNAL_CURRENTPLATFORMCLASS MuleRaspberryPiPlatform
#define MULE_INTERNAL_CURRENTPLATFORMNAME MULE_OTHER_STRINGTYPE("rpi")
#elif defined(MULE_PLATFORM_PIGPIO)
#include "platformsupport/pigpio_rpi/mulepigpioplatform.h"
#define MULE_INTERNAL_CURRENTPLATFORMCLASS MulePigpioPlatform
#define MULE_INTERNAL_CURRENTPLATFORMNAME MULE_OTHER_STRINGTYPE("pigpio")
#else
#error No supported platforms were detected
#endif

class MuleCurrentPlatform
{
public:
    MuleCurrentPlatform();
#ifdef MULE_FEATURES_CORE
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
    bool setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
	return platformClass->setPullUpDown(pin, val);
    }
#endif
#ifdef MULE_FEATURES_FILEIO
    MULE_OTHER_STRINGTYPE readFromFile(MULE_OTHER_STRINGTYPE file) {
        return platformClass->readFromFile(file);
    }
    bool writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
	muledebug("MuleCurrentPlatform::writeToFile(" + file + "," + ct + ") called");
        return platformClass->writeToFile(file, ct);
    }
    bool fileExists(MULE_OTHER_STRINGTYPE file) {
        return platformClass->fileExists(file);
    }
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return platformClass->getPlatformName();
    }
    bool deleteFile(MULE_OTHER_STRINGTYPE file) {
	return platformClass->deleteFile(file);
    }
#endif
#ifdef MULE_FEATURES_SOUND
    void doBeep() {
	platformClass->doBeep();
    }
    bool playWaveFile(MULE_OTHER_STRINGTYPE file) {
	return platformClass->playWaveFile(file);
    }
    MULE_OTHER_STRINGTYPE getSoundBackend() {
	return platformClass->getSoundBackend();
    }
    bool stopAllSounds() {
	return platformClass->stopAllSounds();
    }
#endif
#ifdef MULE_FEATURES_PWMDEVICES
    bool startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) {
	return platformClass->startPWM(pin, dutycycle);
    }
    
    MULE_OTHER_HWPINTYPE getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) {
        return platformClass->getPWMDutyCycle(pin);
    }
    
    MULE_OTHER_HWPINTYPE getPWMRange(MULE_OTHER_HWPINTYPE pin) {
        return platformClass->getPWMRange(pin);
    }
    
    MULE_OTHER_HWPINTYPE getPWMFrequency(MULE_OTHER_HWPINTYPE pin) {
	return platformClass->getPWMFrequency(pin);
    }
    
    bool setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) {
	return platformClass->setPWMRange(pin, range);
    }
    
    bool setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) {
	return platformClass->setPWMFrequency(pin, freq);
    }
#endif
#ifdef MULE_FEATURES_SENSORS
    bool photoresistorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) {
	return platformClass->photoresistorWaitUntilTriggered(pin);
    }
    
    bool buttonWaitUntilPressed(MULE_OTHER_HWPINTYPE pin) {
	return platformClass->buttonWaitUntilPressed(pin);
    }
#endif

private:
    MULE_INTERNAL_CURRENTPLATFORMCLASS* platformClass;
};

#endif // MULECURRENTPLATFORM_H
