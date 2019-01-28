//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "core/muleapplication.h"

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
        MuleApplicationWideData::appWideFirstInstance = this;
        isFirstInstance = true;
        muledebug("This MuleApplication instance was self-appointed as the first one");
    }
    mcpClass = new MULE_INTERNAL_CURRENTPLATFORMCLASS();
    areNecessaryPartsReady = true;
    muledebug("initialized MuleCommonPlatform");
    return true;
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
	return MuleApplicationWideData::appWideFirstInstance;
}

MULE_OTHER_STRINGTYPE MuleApplication::getPlatformName() {
	return mcpClass->getPlatformName();
}

MuleCommonPlatform* MuleApplication::getPlatformClass() {
	return mcpClass;
}

const std::vector<MuleDevice*> MuleApplication::getDevices() {
	return mcpClass->getDevices();
}

MULE_OTHER_STRINGTYPE MuleApplication::getCurrentDirectory() {
	return mulegetcwd();
}

int MuleApplication::exit(int status) {
	internalCleanUp();
#ifndef MULE_INTERNAL_NOEXIT
	std::exit(status);
#endif
	return status;
}

