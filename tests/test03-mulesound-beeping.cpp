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
#include <cstdlib>
#include <iostream>

int main() {
	MuleApplication mApp;
	int ntimebeep = rand() % 10;
	muleprintf("Beeping " + muleinttostr(ntimebeep) + " times");
	for (int i = 0; i < ntimebeep; i++)
		MuleSoundPlayer::beep();
	muleprintf("Goodbye");
	mApp.exit(0);
}
