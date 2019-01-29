//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2019 Tim K <timprogrammer@rambler.ru>
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

#include "mulelinuxgpioplatform.h"

MuleLinuxGPIOPlatform::MuleLinuxGPIOPlatform() {
    if (initialize() == false)
        platformInitializationException(1, "MuleLinuxGPIOPlatform::initialize() returned false, not true");
}


MuleLinuxGPIOPlatform::~MuleLinuxGPIOPlatform() {
	for (int d = 0; d < devlist.size(); d++)
		internal_writeToFile("/sys/class/gpio/unexport", muleinttostr(d));
}

bool MuleLinuxGPIOPlatform::internal_flushPin(const MuleLinuxGPIOPin& pin) {
	MULE_OTHER_STRINGTYPE flushpath = pin.path;
	bool result = true;
	if (pin.input == true)
		result = internal_writeToFile(flushpath + "/direction", "in");
	else
		result = internal_writeToFile(flushpath + "/direction", "out");
	bool result2 = internal_writeToFile(flushpath + "/value", muleinttostr(pin.val));
	if ((result2 == false) || (result == false && result2 == true))
		result = false;
	else
		result = true;
	return result;
	
}

bool MuleLinuxGPIOPlatform::internal_writeToFile(MULE_OTHER_STRINGTYPE fn, MULE_OTHER_STRINGTYPE ct) {
	std::ofstream fileout;
	fileout.open(fn.c_str());
	fileout << ct;
	fileout.close();
	return true;
}

MuleLinuxGPIOPin MuleLinuxGPIOPlatform::internal_readPin(int pn) {
	MuleLinuxGPIOPin toret;
	toret.num = pn;
	MULE_OTHER_STRINGTYPE possiblePinPath = MULE_OTHER_STRINGTYPE("/sys/class/gpio/gpio") + internal_muleIntToStr(pn);
	toret.path = possiblePinPath;
	toret.input = (internal_readToString(possiblePinPath + "/direction") == "in");
	if (internal_stringToNum(internal_readToString(possiblePinPath + "/value")) == 0)
		toret.val = 0;
	else
		toret.val = 1;
	muledebug("generated MuleLinuxGPIOPin for " + muleinttostr(pn));
	muledebug("val = " + muleinttostr(toret.val));
	if (toret.input == true)
		muledebug("input pin");
	else
		muledebug("output pin");
	return toret;
	
}


int MuleLinuxGPIOPlatform::internal_stringToNum(MULE_OTHER_STRINGTYPE in) {
	return std::atoi(in.c_str());
}

MULE_OTHER_STRINGTYPE MuleLinuxGPIOPlatform::internal_readToString(MULE_OTHER_STRINGTYPE fn) {
	std::ifstream stream(fn.c_str());
	return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
}

bool MuleLinuxGPIOPlatform::internal_fileExists(MULE_OTHER_STRINGTYPE fn) {
	std::ifstream* stream = new std::ifstream(fn.c_str());
	bool toret = stream->good();
	delete stream;
	return toret;
}

bool MuleLinuxGPIOPlatform::internal_dirExists(MULE_OTHER_STRINGTYPE dir) {
	return (std::system(MULE_OTHER_STRINGTYPE(MULE_OTHER_STRINGTYPE("test -d \"") + dir + MULE_OTHER_STRINGTYPE("\"")).c_str()) == 0);
}

bool MuleLinuxGPIOPlatform::initialize() {
    if (internal_dirExists(MULE_OTHER_STRINGTYPE("/sys/class/gpio")) == false)
	return false;
    devlist.clear();
    int maxpin = 1;
    if (std::system("cd /sys/class/gpio && for File in gpiochip*/; do  echo $File; done | sed 's+/++g' | tail -1 | sed 's/gpiochip//g' > /tmp/mulelingpio-maxpin.socket") != 0 || internal_fileExists("/tmp/mulelingpio-maxpin.socket") == false)
	return false;
    maxpin = internal_stringToNum(internal_readToString("/tmp/mulelingpio-maxpin.socket"));
    unlink("/tmp/mulelingpio-maxpin.socket");
    for (int i = 0; i < maxpin; i++) {
	internal_writeToFile("/sys/class/gpio/export", muleinttostr(i));
	if (internal_fileExists(MULE_OTHER_STRINGTYPE("/sys/class/gpio/gpio") + muleinttostr(i)) == false)
		return false;
	devlist.push_back(new MuleDevice(i));
    }
    return true;
}

#ifdef MULE_FEATURES_CORE
std::vector<MuleDevice*> MuleLinuxGPIOPlatform::getDevices() {
    return devlist;
}

MULE_OTHER_HWPINTYPE MuleLinuxGPIOPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    if (internal_readPin(pin).input == true)
	return MULE_INPUT;
    return MULE_OUTPUT;
}

bool MuleLinuxGPIOPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("mode = " + muleinttostr((int)(mode)));
    MuleLinuxGPIOPin pin_s = internal_readPin(pin);
    pin_s.input = (mode == MULE_INPUT);
    return internal_flushPin(pin_s);
}

MULE_OTHER_HWPINTYPE MuleLinuxGPIOPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    return internal_readPin(pin).val;
}

bool MuleLinuxGPIOPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("ct = " + muleinttostr((int)(ct)));
    MuleLinuxGPIOPin pin_s = internal_readPin(pin);
    pin_s.val = ct;
    return internal_flushPin(pin_s);
}

bool MuleLinuxGPIOPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("val = " + muleinttostr((int)(val)));
    muledebug("unsupported function, return writeToPin");
    return writeToPin(pin, val);
}
#endif


#ifdef MULE_FEATURES_SENSORS
bool MuleLinuxGPIOPlatform::sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) {
	int orval = internal_readPin(pin).val;
	while (orval == internal_readPin(pin).val)
		muledebug("waiting for trigger");
	return true;
}
#endif

#ifdef MULE_FEATURES_FILEIO
MULE_OTHER_STRINGTYPE MuleLinuxGPIOPlatform::readFromFile(MULE_OTHER_STRINGTYPE file) {
    muledebug("file = " + file);
    std::ifstream t(file.c_str());
    return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(t)),
                                    std::istreambuf_iterator<char>());
}

bool MuleLinuxGPIOPlatform::writeToFile(MULE_OTHER_STRINGTYPE file, MULE_OTHER_STRINGTYPE ct) {
    muledebug("MuleLinuxGPIOPlatform::writeToFile(" + file + "," + ct + ") called");
    try {
	    std::ofstream stream(file.c_str());
	    stream << ct;
	    stream.close();
	    return true;
    }
    catch (...) {
	    muledebug("try catch failed");
    }
    return false;
}

bool MuleLinuxGPIOPlatform::fileExists(MULE_OTHER_STRINGTYPE file) {
    struct stat buffer;
    return (stat (file.c_str(), &buffer) == 0);
}

bool MuleLinuxGPIOPlatform::deleteFile(MULE_OTHER_STRINGTYPE file) {
    return (unlink(file.c_str()) == 0);
}
#endif


#ifdef MULE_FEATURES_SOUND
void MuleLinuxGPIOPlatform::doBeep() {
	std::system("aplay -d 1 /dev/urandom");
}

bool MuleLinuxGPIOPlatform::playWaveFile(MULE_OTHER_STRINGTYPE filename) {
	if (fileExists(filename) == false)
		return false;
	if (std::system(MULE_OTHER_STRINGTYPE("aplay " + filename).c_str()) == 0)
		return true;
	else
		return false;
	return false;
}

bool MuleLinuxGPIOPlatform::stopAllSounds() {
	if (std::system("killall aplay") == 0)
		return true;
	else
		return false;
}
#endif

