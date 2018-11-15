#include "mulepigpioplatform.h"

MulePigpioPlatform::MulePigpioPlatform() {
    if (initialize() == false)
        platformInitializationException(1, "MulePigpioPlatform::initialize() returned false, not true");
}

bool MulePigpioPlatform::initialize() {
	if (gpioInitialise() < 0)
		return false;
	devlist.clear();
	for (int i = 0; i < 54; i++)
		devlist.push_back(new MuleDevice(i));
	return true;
}

#ifdef MULE_FEATURES_SENSORS
std::vector<MuleDevice*> MulePigpioPlatform::getDevices() {
    return devlist;
}

MULE_OTHER_HWPINTYPE MulePigpioPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    return gpioGetMode(pin);
}

bool MulePigpioPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
    muledebug("pin = " + muleinttostr((int)(mode)));
    muledebug("mode = " + muleinttostr((int)(mode)));
    if (gpioSetMode(pin, mode) == 0)
	return true;
    return false;
}

MULE_OTHER_HWPINTYPE MulePigpioPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    return gpioRead(pin);
}

bool MulePigpioPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    // this function is based on gpioWrite() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("ct = " + muleinttostr((int)(ct)));
    if (gpioWrite(pin, ct) == 0)
	return true;
    return false;
}

bool MulePigpioPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    // this function is based on gpioSetPullUpDown() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("val = " + muleinttostr((int)(val)));
    if (gpioSetPullUpDown(pin, val) == 0)
	return true;
    return false;
}
#endif

#ifdef MULE_FEATURES_FILEIO
MULE_OTHER_STRINGTYPE MulePigpioPlatform::readFromFile(MULE_OTHER_STRINGTYPE file) {
    muledebug("file = " + file);
    std::ifstream t(file.c_str());
    return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(t)),
                                    std::istreambuf_iterator<char>());
}

bool MulePigpioPlatform::writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
    muledebug("MulePigpioPlatform::writeToFile(" + file + "," + ct + ") called");
    try {
	    std::ofstream stream(file.c_str());
	    stream << ct;
	    stream.close();
	    return true;
    }
    catch (...) {
	    muledebug("try catch failed");
    }
    return false;
}

bool MulePigpioPlatform::fileExists(MULE_OTHER_STRINGTYPE file) {
    struct stat buffer;
    return (stat (file.c_str(), &buffer) == 0);
}

bool MulePigpioPlatform::deleteFile(MULE_OTHER_STRINGTYPE file) {
	if (std::system(MULE_OTHER_STRINGTYPE("rm -r -f " + file).c_str()) == 0)
		return true;
	else
		return false;
}
#endif


#ifdef MULE_FEATURES_SOUND
void MulePigpioPlatform::doBeep() {
	std::system("aplay -d 1 /dev/urandom");
}

bool MulePigpioPlatform::playWaveFile(MULE_OTHER_STRINGTYPE filename) {
	if (fileExists(filename) == false)
		return false;
	if (std::system(MULE_OTHER_STRINGTYPE("aplay " + filename).c_str()) == 0)
		return true;
	else
		return false;
	return false;
}

bool MulePigpioPlatform::stopAllSounds() {
	if (std::system("killall aplay") == 0)
		return true;
	else
		return false;
}
#endif

