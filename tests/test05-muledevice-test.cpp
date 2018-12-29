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

#include <mule.h>

int main() {
	MuleApplication mApp;
	int returncode = 0;
	muleprintf("libMule running on %s\n", mApp.getPlatformName().c_str());
	muleprintf("init MuleDevice\n");
	MuleDevice mDev(1);
	muleprintf("mDev.read() = " + muleinttostr(mDev.read()) + "\n");
	if (mDev.write(1) == true && mDev.read() == 1)
		muleprintf("write success");
	else {
		muleprintf("write fail");
		returncode = 1;
	}
	mulesleep(2);
	if (mDev.write(0) == true && mDev.read() == 0)
		muleprintf("write 0 success");
	else {
		muleprintf("write 0 fail");
		returncode = 2;
	}
	mulesleep(0.2);
	if (mDev.trigger(5000000, 1) == true)
		muleprintf("trigger success");
	else {
		muleprintf("trigger fail");
		returncode = 3;
	}
	muleprintf("goodbye");
	return mApp.exit(returncode);
}
