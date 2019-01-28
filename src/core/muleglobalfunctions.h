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

#ifndef MULEGLOBALFUNCTIONS_H
#define MULEGLOBALFUNCTIONS_H
#include "core/muleconfig.h"
#include "core/muleplatformdependantfunctions.h"
#include <iostream>
#include <sstream>
#ifndef MULE_INTERNAL_NOEXCEPTIONS
#include <exception>
#endif
#include <stdlib.h>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <unistd.h>

void mulestrmodreplace(MULE_OTHER_STRINGTYPE& subject, MULE_OTHER_STRINGTYPE search, MULE_OTHER_STRINGTYPE replace);
MULE_OTHER_STRINGTYPE muleyesno(bool in);
void muleprintf(MULE_OTHER_STRINGTYPE in, ...);
MULE_OTHER_STRINGTYPE muleinttostr(int in);
void muledebug(MULE_OTHER_STRINGTYPE debugout);
MULE_OTHER_STRINGTYPE mulegetcwd();
void muleexception(int errcode, MULE_OTHER_STRINGTYPE errtext, bool cancatch = true);
void mulesleep(double seconds);
void mulemicrosecsleep(int microseconds);
#endif // MULEGLOBALFUNCTIONS_H
