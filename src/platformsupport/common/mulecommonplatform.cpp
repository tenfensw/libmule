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

#include "platformsupport/common/mulecommonplatform.h"

MuleCommonPlatform::~MuleCommonPlatform() {
	internal_cleanDevList();
}

MULE_OTHER_STRINGTYPE MuleCommonPlatform::getPlatformName() {
	return "unknown";
}

std::vector<MuleDevice*> MuleCommonPlatform::getDevices() {
	return devlist;
}

void MuleCommonPlatform::internal_cleanDevList() {
	for (int d = 0; d < devlist.size(); d++) {
		delete devlist.at(d);
		devlist.at(d) = NULL;
	}
	devlist.clear();
}

void MuleCommonPlatform::platformInitializationException(int erc, MULE_OTHER_STRINGTYPE message) {
#ifdef MULE_INTERNAL_PRINTINTERNALERRORSLCD
        muleprintf("libMule " + muleinttostr(MULE_VERSION_MAJOR) + "." + muleinttostr(MULE_VERSION_MINOR) + "." + muleinttostr(MULE_VERSION_UPDATE) + " Platform Initialization Error\n");
        muleprintf( "A critical error occured during the initialization of the device platform.\n");
        muleprintf("\n");
        muleprintf("Error №" + muleinttostr(erc) + ": " + message);
        muleprintf("\n\n");
        muleprintf("The application will now exit now.\n");
#endif
        int exitcode = 400 + erc;
	std::exit(exitcode);
}

