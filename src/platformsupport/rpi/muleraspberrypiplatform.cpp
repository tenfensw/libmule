#include "muleraspberrypiplatform.h"

MuleRaspberryPiPlatform::MuleRaspberryPiPlatform() {
    if (initialize() == false)
        platformInitializationException(1, "MuleRaspberryPiPlatform::initialize() returned false, not true");
}

bool MuleRaspberryPiPlatform::initialize() {
	// this function is based on gpioInitialise() function from tiny_gpio.c
	int fd;
	fd = open("/dev/gpiomem", O_RDWR | O_SYNC) ;
	if (fd < 0) {
		muledebug("Could not open /dev/gpiomem");
		return false;
	}
	gpioReg = (uint32_t *)mmap(NULL, 0xB4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	if (gpioReg == MAP_FAILED) {
		muledebug("Failed to set gpioReg variable");
		return false;
	}
	devlist.clear();
	for (int i = 0; i < 54; i++)
		devlist.push_back(new MuleDevice(i));
	return true;
}

#ifdef MULE_FEATURES_SENSORS
std::vector<MuleDevice*> MuleRaspberryPiPlatform::getDevices() {
    return devlist;
}

MULE_OTHER_HWPINTYPE MuleRaspberryPiPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
    // this function is based on gpioGetMode() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(pin)));
    int reg, shift;
    reg  = pin/10;
    shift = (pin%10) * 3;
    return (*(gpioReg + reg) >> shift) & 7;
}

bool MuleRaspberryPiPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
    // this function is based on gpioSetMode() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(mode)));
    muledebug("mode = " + muleinttostr((int)(mode)));
    try {
	int reg = pin/10;
	int shift = (pin%10) * 3;
	gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
	return true;
    }
    catch (...) {
	muledebug("Caught an ASSERT, SEGFAULT or an exception. return false;");
    }
    return false;
}

MULE_OTHER_HWPINTYPE MuleRaspberryPiPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    // this function is based on gpioRead() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(pin)));
    if ((*(gpioReg + GPLEV0 + PI_BANK) & PI_BIT) != 0)
	return 1;
    else
	return 0;
}

bool MuleRaspberryPiPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    // this function is based on gpioWrite() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("ct = " + muleinttostr((int)(ct)));
    try {
	    if (ct == 0)
		*(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
	    else
		*(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
	    return true;
    }
    catch (...) {
            muledebug("Caught an ASSERT, SEGFAULT or an exception. return false;");
    }
    return false;
}

bool MuleRaspberryPiPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    // this function is based on gpioSetPullUpDown() function from tiny_gpio.c
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("val = " + muleinttostr((int)(val)));
    try {
	*(gpioReg + GPPUD) = val;
	mulemicrosecsleep(20);
	*(gpioReg + GPPUDCLK0 + PI_BANK) = PI_BIT;
	mulemicrosecsleep(20);
	*(gpioReg + GPPUD) = 0;
	*(gpioReg + GPPUDCLK0 + PI_BANK) = 0;
	return true;
    }
    catch (...) {
    	muledebug("setPullUpDown failed, return false");
    }
    return false;
}
#endif

#ifdef MULE_FEATURES_FILEIO
MULE_OTHER_STRINGTYPE MuleRaspberryPiPlatform::readFromFile(MULE_OTHER_STRINGTYPE file) {
    muledebug("file = " + file);
    std::ifstream t(file.c_str());
    return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(t)),
                                    std::istreambuf_iterator<char>());
}

bool MuleRaspberryPiPlatform::writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
    muledebug("MuleRaspberryPiPlatform::writeToFile(" + file + "," + ct + ") called");
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

bool MuleRaspberryPiPlatform::fileExists(MULE_OTHER_STRINGTYPE file) {
    struct stat buffer;
    return (stat (file.c_str(), &buffer) == 0);
}

bool MuleRaspberryPiPlatform::deleteFile(MULE_OTHER_STRINGTYPE file) {
	if (std::system(MULE_OTHER_STRINGTYPE("rm -r -f " + file).c_str()) == 0)
		return true;
	else
		return false;
}
#endif


#ifdef MULE_FEATURES_SOUND
void MuleRaspberryPiPlatform::doBeep() {
	std::system("aplay -d 1 /dev/urandom");
}

bool MuleRaspberryPiPlatform::playWaveFile(MULE_OTHER_STRINGTYPE filename) {
	if (fileExists(filename) == false)
		return false;
	if (std::system(MULE_OTHER_STRINGTYPE("aplay " + filename).c_str()) == 0)
		return true;
	else
		return false;
	return false;
}

bool MuleRaspberryPiPlatform::stopAllSounds() {
	if (std::system("killall aplay") == 0)
		return true;
	else
		return false;
}
#endif

