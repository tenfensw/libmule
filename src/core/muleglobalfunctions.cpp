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

#include "core/muleglobalfunctions.h"

void mulestrmodreplace(MULE_OTHER_STRINGTYPE& subject, MULE_OTHER_STRINGTYPE search,
                          MULE_OTHER_STRINGTYPE replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != MULE_OTHER_STRINGTYPE::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}
MULE_OTHER_STRINGTYPE muleyesno(bool in) {
    if (in == true)
        return "yes";
    else
        return "no";
}

void muleprintf(MULE_OTHER_STRINGTYPE in, ...) {
    va_list otherargs;
    va_start(otherargs, in);
#ifdef MULE_FEATURES_SUPPORTSCOUT
    vprintf(in.c_str(), otherargs);
#endif
    muleplatformprintf(in, otherargs);
    va_end(otherargs);
    return;
}

MULE_OTHER_STRINGTYPE muleinttostr(int in) {
#ifndef MULE_INTERNAL_NOSTRINGSTREAMS
	std::ostringstream ostr;
	ostr << in;
	return ostr.str();
#else
	char buf[70];
	itoa(in, buf, 10);
	return MULE_OTHER_STRINGTYPE(buf);
#endif
}

void muledebug(MULE_OTHER_STRINGTYPE debugout) {
#ifdef MULE_FEATURES_DEBUG
    muleprintf("[DEBUG] " + debugout + "\n");
#endif
    return;
}

MULE_OTHER_STRINGTYPE mulegetcwd() {
#ifdef MULE_FEATURES_FILEIO
    char wtw[1600];
    getcwd(wtw, 1600);
    return MULE_OTHER_STRINGTYPE(wtw);
#else
    return MULE_OTHER_STRINGTYPE("/undefined");
#endif
}


void muleexception(int errcode, MULE_OTHER_STRINGTYPE errtext, bool cancatch) {
    muleprintf("libMule Application Exception\n");
    muleprintf("An error occured while running the code of current application.\n");
    muleprintf("\n");
    muleprintf("Error code: %d\n", errcode);
    muleprintf("Message: %s\n", errtext.c_str());
    muleprintf("Critical: %s\n", muleyesno(cancatch).c_str());
    muleprintf("\n");
    if (cancatch == true) {
        muleprintf("This program will continue to run if the algorithm catches this exception.\n");
#ifndef MULE_INTERNAL_NOEXCEPTIONS
        std::exception();
#else
	muleprintf("Your microcontroller does not support exceptions, so the app will continue to work, but bugs and glitches might occur.");
#endif
    }
    else {
        muleprintf("This program will now close because this exception is critical.\n");
        exit(errcode + 500);
    }
    return;
}


void mulesleep(double seconds) {
	muleplatformsleep(seconds);
}

void mulemicrosecsleep(int microseconds) {
	mulesleep(((1/100000) * microseconds));
}

