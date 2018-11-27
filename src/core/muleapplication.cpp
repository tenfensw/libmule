#include "core/muleapplication.h"

namespace MuleApplicationWideData {
	void* appWideFirstInstance;
};

MuleApplication::MuleApplication()
{
    areNecessaryPartsReady = false;
    if (internalInit())
        muledebug("MuleApplication was successfully initialized");
    else
        muleexception(9, "MuleApplication could not be initialized", false);
}

MuleApplication::~MuleApplication() {
    this->exit(0);
}

bool MuleApplication::internalInit() {
    isFirstInstance = false;
    if (!(MuleApplicationWideData::appWideFirstInstance)) {
        MuleApplicationWideData::appWideFirstInstance = (void*)this;
        isFirstInstance = true;
        muledebug("This MuleApplication instance was self-appointed as the first one");
    }
    try {
        mcpClass = new MuleCurrentPlatform();
	areNecessaryPartsReady = true;
        muledebug("MuleCurrentPlatform initialized");
        return true;
    } catch (...) {
        return false;
    }
}

void MuleApplication::internalCleanUp() {
    delete mcpClass;
    if (isFirstInstance == true)
#ifndef MULE_FEATURES_SUPPORTSNULLPTR
        MuleApplicationWideData::appWideFirstInstance = NULL;
#else
        MuleApplicationWideData::appWideFirstInstance = nullptr;
#endif
}

MuleApplication* MuleApplication::getRunningInstance() {
	return (MuleApplication*)(MuleApplicationWideData::appWideFirstInstance);
}

MULE_OTHER_STRINGTYPE MuleApplication::getPlatformName() {
	return mcpClass->getPlatformName();
}

MuleCurrentPlatform* MuleApplication::getPlatformClass() {
	return mcpClass;
}

std::vector<MuleDevice*> MuleApplication::getDevices() {
	return mcpClass->getDevices();
}

MULE_OTHER_STRINGTYPE MuleApplication::getCurrentDirectory() {
	return mulegetcwd();
}

int MuleApplication::exit(int status) {
	internalCleanUp();
	std::exit(status);
	return status;
}

