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
	muleprintf("Testing MuleMultipinDevice, ");
	MuleMultipinDevice device1(0, 2, 1);
	muleprintf("device1 initialized successfully, ");
	if (device1.getPin(1)->getPin() == 1)
		muleprintf("it returned pin 1 properly, ");
	else {
		returncode = returncode + 1;
		muleprintf("did not return pin 1 properly, ");
	}
	if (device1.getIndex(1)->getPin() == 2)
		muleprintf("it also returned index 1 properly, ");
	else {
		returncode = returncode + 1;
		muleprintf("did not return index 1 properly, ");
	}
	muleprintf("add/remove test was invoked\n");
	int orcount = device1.count();
	int ncount = 0;
	muleprintf("currently orcount = %d\n", orcount);
	muleprintf("after removing pin 2 it became ");
	device1.removePin(2);
	ncount = device1.count();
	muleprintf("%d, ", ncount);
	if (ncount == orcount) {
		returncode = returncode + 1; 
		muleprintf("that's bad, this was not supposed to happen\n");
	}
	else
		muleprintf("that's awesome, continuing\n");
	orcount = ncount;
	ncount = 0;
	muleprintf("after adding MuleDevice(3), it became ");
	device1.addPin(MuleDevice(3));
	ncount = device1.count();
	if (orcount == ncount) {
		returncode = returncode + 1; 
		muleprintf("... it stayed the same, that's bad\n");
	}
	else
		muleprintf("%d, that's very good\n", ncount);
	if (returncode == 0)
		muleprintf("so far, it works\n");
	else if (returncode == 1)
		muleprintf("it's a bit buggy, but still kinda works\n");
	else
		muleprintf("it's extremely buggy, gotta fix all the bugs\n");
	muleprintf("goodbye\n");
	return mApp.exit(returncode);
}