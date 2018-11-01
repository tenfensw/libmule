#ifndef MULEGLOBALFUNCTIONS_H
#define MULEGLOBALFUNCTIONS_H
#include <iostream>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <unistd.h>
#include "core/muleconfig.h"
#ifdef MULE_PLATFORM_LEGOEV3
#include <ev3.h>
#endif

void mulestrmodreplace(MULE_OTHER_STRINGTYPE& subject, const MULE_OTHER_STRINGTYPE& search, const MULE_OTHER_STRINGTYPE& replace);
MULE_OTHER_STRINGTYPE muleyesno(const bool& in);
void muleprintf(MULE_OTHER_STRINGTYPE in);
MULE_OTHER_STRINGTYPE muleinttostr(int in);
void muledebug(MULE_OTHER_STRINGTYPE debugout);
MULE_OTHER_STRINGTYPE mulegetcwd();
void muleexception(const int& errcode, const MULE_OTHER_STRINGTYPE& errtext, const bool& cancatch);
void mulesleep(const double& seconds);
#endif // MULEGLOBALFUNCTIONS_H
