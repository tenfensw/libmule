#include "muleapplication.h"

MuleApplication::MuleApplication()
{
    if (internalInit())
        muledebug("MuleApplication was successfully initialized");
    else
        muleexception(9, "MuleApplication could not be initialized", false);
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
        muledebug("MuleCurrentPlatform initialized");
        return true;
    } catch (...) {
        return false;
    }
}

MuleApplication::~MuleApplication() {
    this->exit(0);
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
