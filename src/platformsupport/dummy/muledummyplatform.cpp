#include "muledummyplatform.h"

MuleDummyPlatform::MuleDummyPlatform() {
    if (initialize() == false)
        platformInitializationException(1, "MuleDummyPlatform::initialize() returned false, not true");
}

bool MuleDummyPlatform::initialize() {
    int devcount = rand() % 100;
    for (int i = 0; i < devcount; i++) {
        MuleDevice* md = new MuleDevice((MULE_OTHER_HWPINTYPE)(i));
        devlist.push_back(md);
    }
    return true;
}

#ifdef MULE_FEATURES_SENSORS
std::vector<MuleDevice*> MuleDummyPlatform::getDevices() {
    return devlist;
}

MULE_OTHER_HWPINTYPE MuleDummyPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    platformInitializationException(2, "Unimplemented");
    return 0;
}

bool MuleDummyPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("mode = " + muleinttostr((int)(mode)));
    platformInitializationException(2, "Unimplemented");
    return false;
}

MULE_OTHER_HWPINTYPE MuleDummyPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    platformInitializationException(2, "Unimplemented");
    return 0;
}

bool MuleDummyPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("ct = " + muleinttostr((int)(ct)));
    platformInitializationException(2, "Unimplemented");
    return false;
}

bool MuleDummyPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("val = " + muleinttostr((int)(val)));
    platformInitializationException(2, "Unimplemented");
    return false;
}
#endif

#ifdef MULE_FEATURES_FILEIO
MULE_OTHER_STRINGTYPE MuleDummyPlatform::readFromFile(MULE_OTHER_STRINGTYPE file) {
    muledebug("file = " + file);
    std::ifstream t(file.c_str());
    return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(t)),
                                    std::istreambuf_iterator<char>());
}

bool MuleDummyPlatform::writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
    muledebug("MuleDummyPlatform::writeToFile(" + file + "," + ct + ") called");
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

bool MuleDummyPlatform::fileExists(MULE_OTHER_STRINGTYPE file) {
    struct stat buffer;
    return (stat (file.c_str(), &buffer) == 0);
}

bool MuleDummyPlatform::deleteFile(MULE_OTHER_STRINGTYPE file) {
#ifdef MULE_OS_UNIX
	if (std::system(MULE_OTHER_STRINGTYPE("rm -r -f " + file).c_str()) == 0)
		return true;
	else
		return false;
#else
	return false;
#endif
}
#endif


#ifdef MULE_FEATURES_SOUND
void MuleDummyPlatform::doBeep() {
	std::system("beep || aplay -d 1 /dev/urandom || exit 1");
}

bool MuleDummyPlatform::playWaveFile(MULE_OTHER_STRINGTYPE filename) {
	if (fileExists(filename) == false)
		return false;
	if (std::system(MULE_OTHER_STRINGTYPE("aplay " + filename).c_str()) == 0 || std::system(MULE_OTHER_STRINGTYPE("paplay " + filename).c_str()) == 0 || std::system(MULE_OTHER_STRINGTYPE("cat " + filename + " > /dev/dsp").c_str()) == 0)
		return true;
	else
		return false;
	return false;
}

bool MuleDummyPlatform::stopAllSounds() {
	if (std::system("killall aplay; killall paplay; killall cat") == 0)
		return true;
	else
		return false;
}
#endif

#ifdef MULE_FEATURES_PWMDEVICES
bool MuleDummyPlatform::startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) {
	return false;
}

MULE_OTHER_HWPINTYPE MuleDummyPlatform::getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) {
	return -1;
}

MULE_OTHER_HWPINTYPE MuleDummyPlatform::getPWMRange(MULE_OTHER_HWPINTYPE pin) {
	return -1;
}

bool MuleDummyPlatform::setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) {
	return false;
}

MULE_OTHER_HWPINTYPE MuleDummyPlatform::getPWMFrequency(MULE_OTHER_HWPINTYPE pin) {
	return -1;
}

bool MuleDummyPlatform::setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) {
	return false;
}
#endif


