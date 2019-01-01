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

#include "core/muleconfig.h"
#include "core/muleapplication.h"
#include "core/muledevice.h"
#include "core/muleglobalfunctions.h"
#include "platformsupport/common/mulecommonplatform.h"
#include "platformsupport/common/mulecurrentplatform.h"
#ifdef MULE_PLATFORM_DUMMY
#include "platformsupport/dummy/muledummyplatform.h"
#elif defined(MULE_PLATFORM_LEGOEV3)
#include "platformsupport/legoev3/mulemindstormsplatform.h"
#elif defined(MULE_PLATFORM_PIGPIO)
#include "platformsupport/pigpio_rpi/mulepigpioplatform.h"
#elif defined(MULE_PLATFORM_ARDUINOAVR)
#include "platformsupport/arduinoavr/mulearduinoavrplatform.h"
#elif defined(MULE_PLATFORM_MICROCONTROLLERSIM)
#include "platformsupport/microcontrollersim/mulemicrocontrollersimplatform.h"
#endif
#ifdef MULE_FEATURES_FILEIO
#include "fileio/mulefile.h"
#include "fileio/muledirectoryfunctions.h"
#endif
#ifdef MULE_FEATURES_SOUND
#include "sound/mulesoundplayer.h"
#endif
#ifdef MULE_FEATURES_PWMDEVICES
#include "pwmdevices/mulepwmdevice.h"
#include "pwmdevices/muleservodevice.h"
#endif
#ifdef MULE_FEATURES_SENSORS
#include "sensors/mulesensor.h"
#endif

