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
    MuleCurrentPlatform* getPlatformClass();
    std::vector<MuleDevice*> getDevices();
    MULE_OTHER_STRINGTYPE getCurrentDirectory();
    static MuleApplication* getRunningInstance();
    void exit(const int& status);
    bool areNecessaryPartsReady;

private:
    MuleCurrentPlatform* mcpClass;
    bool isFirstInstance;
    bool internalInit();
    void internalCleanUp();

};



#endif // MULEAPPLICATION_H
