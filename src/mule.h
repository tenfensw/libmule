// mule.h - global include header for libMule
// Part of libMule
// Copyright (c) Tim K (timprogrammer@rambler.ru) 2018. Licensed under GNU GPLv3.

#include "core/muleapplication.h"
#include "core/muledevice.h"
#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"
#include "platformsupport/common/mulecommonplatform.h"
#include "platformsupport/common/mulecurrentplatform.h"
#ifdef MULE_PLATFORM_DUMMY
#include "platformsupport/dummy/muledummyplatform.h"
#elif defined(MULE_PLATFORM_LEGOEV3)
#include "platformsupport/dummy/mulemindstormsplatform.h"
#endif
