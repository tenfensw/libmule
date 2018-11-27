#ifndef MULEAPPLICATION_H
#define MULEAPPLICATION_H

#include <string>
#include <vector>
#include <cstdlib>
#include <csignal>
#include "platformsupport/common/mulecurrentplatform.h"
#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"

namespace MuleApplicationWideData {
	extern void* appWideFirstInstance;
};

class MuleApplication
{   

public:
    MuleApplication();
    ~MuleApplication();
    MULE_OTHER_STRINGTYPE getPlatformName();
    MULE_OTHER_STRINGTYPE platformName() { return this->getPlatformName(); }
    MuleCurrentPlatform* getPlatformClass();
    MuleCurrentPlatform* platformClass() { return this->getPlatformClass(); }
    std::vector<MuleDevice*> getDevices();
    std::vector<MuleDevice*> devices() { return this->getDevices(); }
    MULE_OTHER_STRINGTYPE getCurrentDirectory();
    MULE_OTHER_STRINGTYPE currentDirectory() { return this->getCurrentDirectory(); }
    static MuleApplication* getRunningInstance();
    int exit(int status);
    bool areNecessaryPartsReady;

private:
    MuleCurrentPlatform* mcpClass;
    bool isFirstInstance;
    bool internalInit();
    void internalCleanUp();

};



#endif // MULEAPPLICATION_H
