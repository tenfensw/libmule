#ifndef MULEGLOBALFUNCTIONS_H
#define MULEGLOBALFUNCTIONS_H
#include <iostream>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include "core/muleconfig.h"
#ifdef MULE_PLATFORM_LEGOEV3
#include <ev3.h>
#endif

void mulestrmodreplace(MULE_OTHER_STRINGTYPE& subject, MULE_OTHER_STRINGTYPE search, MULE_OTHER_STRINGTYPE replace);
MULE_OTHER_STRINGTYPE muleyesno(bool in);
void muleprintf(MULE_OTHER_STRINGTYPE in, ...) const;
MULE_OTHER_STRINGTYPE muleinttostr(int in);
void muledebug(MULE_OTHER_STRINGTYPE debugout);
MULE_OTHER_STRINGTYPE mulegetcwd();
void muleexception(int errcode, MULE_OTHER_STRINGTYPE errtext, bool cancatch);
void mulesleep(double seconds);
void mulemicrosecsleep(int microseconds);
#endif // MULEGLOBALFUNCTIONS_H
