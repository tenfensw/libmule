//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018-2019 Tim K <timprogrammer@rambler.ru>
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

#ifndef MULEAPPLICATION_H
#define MULEAPPLICATION_H

#include "core/muleconfig.h"
#include "platformsupport/common/mulecommonplatform.h"
#include MULE_OTHER_NATIVEPLATFORMHEADER
#include <string>
#include <vector>
#include <cstdlib>
#ifndef MULE_INTERNAL_NOEXIT
#include <csignal>
#endif
#include "core/muleglobalfunctions.h"
#include "core/muledevice.h"


class MuleApplication
{   

public:
    MuleApplication();
    ~MuleApplication();
    MULE_OTHER_STRINGTYPE getPlatformName();
    MULE_OTHER_STRINGTYPE platformName() { return getPlatformName(); }
    MuleCommonPlatform* getPlatformClass();
    MuleCommonPlatform* platformClass() { return getPlatformClass(); }
    const std::vector<MuleDevice*> getDevices();
    const std::vector<MuleDevice*> devices() { return getDevices(); }
    MULE_OTHER_STRINGTYPE getCurrentDirectory();
    MULE_OTHER_STRINGTYPE currentDirectory() { return getCurrentDirectory(); }
    static MuleApplication* getRunningInstance();
    int exit(int status);
    bool areNecessaryPartsReady() const { return (mcpClass != nullptr); }

private:
    MuleCommonPlatform* mcpClass;
    bool isFirstInstance;
    bool internalInit();
    void internalCleanUp();

};



#endif // MULEAPPLICATION_H
