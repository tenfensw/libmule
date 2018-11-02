// Some functions from this class are heavily based on a little C program called tiny_gpio.c. It was created by the same author, 
// who made the pigpio library. The source code of tiny_gpio.c can be viewed here: http://abyz.me.uk/rpi/pigpio/examples.html
// That code was released under a public domain license, but I could not resist to mention about tiny_gpio.c here, as it helped
// me a lot during the creation of libMule.

#ifndef MULERASPBERRYPIPLATFORM_H
#define MULERASPBERRYPIPLATFORM_H

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

#define MULE_HOMEDIRECTORY "/home/" + MULE_OTHER_STRINGTYPE(getenv("USER"))
#define MULE_DOCUMENTSDIRECTORY MULE_HOMEDIRECTORY + "/Documents"
#define MULE_TEMPDIRECTORY "/tmp"

// Raspberry Pi-related defines
#define GPSET0	7
#define GPSET1	8
#define GPCLR0	10
#define GPCLR1	11
#define GPLEV0	13
#define GPLEV1	14
#define GPPUD	37
#define GPPUDCLK0	38
#define GPPUDCLK1	39
#define PI_BANK	(pin>>5)
#define PI_BIT	(1<<(pin&0x1F))
#define PI_INPUT	0
#define PI_OUTPUT	1
#define PI_ALT0		4
#define PI_ALT1		5
#define PI_ALT2		6
#define PI_ALT3		7
#define PI_ALT4		3
#define PI_ALT5		2
#define PI_PUD_OFF	0
#define PI_PUD_DOWN	1
#define PI_PUD_UP	2


class MuleRaspberryPiPlatform : public MuleCommonPlatform
{
public:
    MuleRaspberryPiPlatform();

    bool initialize();
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return "rpi";
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
    volatile uint32_t *gpioReg; 
};

#endif // MULERASPBERRYPIPLATFORM_H
