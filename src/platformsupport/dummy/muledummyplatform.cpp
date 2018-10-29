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
#endif

#ifdef MULE_FEATURES_FILEIO
MULE_OTHER_STRINGTYPE MuleDummyPlatform::readFromFile(MULE_OTHER_STRINGTYPE file) {
    muledebug("file = " + file);
    std::ifstream t(file);
    return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(t)),
                                    std::istreambuf_iterator<char>());
}

bool MuleDummyPlatform::writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
    if (fileExists(file) == false)
        return false;
    std::ofstream stream(file);
    stream << ct;
    stream.close();
    return true;
}

bool MuleDummyPlatform::fileExists(MULE_OTHER_STRINGTYPE file) {
    struct stat buffer;
    return (stat (file.c_str(), &buffer) == 0);
}
#endif
