#ifndef MULEAPPLICATION_H
#define MULEAPPLICATION_H

#include <string>
#include <vector>
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
    MULE_OTHER_STRINGTYPE getPlatformName() {
        return mcpClass->getPlatformName();
    }
    MuleCurrentPlatform* getPlatformClass() {
        return mcpClass;
    }
    std::vector<MuleDevice*> getDevices() {
        return mcpClass->getDevices();
    }
    MULE_OTHER_STRINGTYPE getCurrentDirectory() {
        return mulegetcwd();
    }
    static MuleApplication* getRunningInstance();
    void exit(const int& status) {
        internalCleanUp();
        exit(status);
    }

private:
    MuleCurrentPlatform* mcpClass;
    bool isFirstInstance;
    bool internalInit();
    void internalCleanUp();

};



#endif // MULEAPPLICATION_H
