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

#include "mulemicrocontrollersimplatform.h"

MuleMicrocontrollerSimulatorPlatform::MuleMicrocontrollerSimulatorPlatform() {
    if (initialize() == false)
        platformInitializationException(1, "MuleMicrocontrollerSimulatorPlatform::initialize() returned false, not true");
}

MULE_OTHER_STRINGTYPE MuleMicrocontrollerSimulatorPlatform::internal_muleIntToStr(int in) {
	std::stringstream stream;
	stream << in;
	return stream.str();
}

bool MuleMicrocontrollerSimulatorPlatform::internal_flushPin(const MuleMicrocontrollerSimulatorPin& pin) {
	MULE_OTHER_STRINGTYPE towrite = "";
	MULE_OTHER_STRINGTYPE flushpath = pin.path;
	if (pin.digital == true)
		towrite = towrite + "type=d,";
	else
		towrite = towrite + "type=a,";
	if (pin.input == true)
		towrite = towrite + "inout=i,";
	else
		towrite = towrite + "inout=o,";
	if (pin.pud == MULE_PUD_OFF)
		towrite = towrite + "pud=off,";
	else if (pin.pud == MULE_PUD_UP)
		towrite = towrite + "pud=up,";
	else
		towrite = towrite + "pud=down,";
	towrite = towrite + "val=" + internal_muleIntToStr(pin.val) + ",dc=" + internal_muleIntToStr(pin.dc) + ",maxdc=" + internal_muleIntToStr(pin.maxdc) + ",";
	if (pin.pwm == true)
		towrite = towrite + "pwm=yes";
	else
		towrite = towrite + "pwm=no";
	towrite = towrite + ",freq=" + internal_muleIntToStr(pin.freq);
	return internal_writeToFile(flushpath, towrite);
	
}

bool MuleMicrocontrollerSimulatorPlatform::internal_writeToFile(MULE_OTHER_STRINGTYPE fn, MULE_OTHER_STRINGTYPE ct) {
	std::ofstream fileout;
	fileout.open(fn.c_str());
	fileout << ct;
	fileout.close();
	return true;
}

MuleMicrocontrollerSimulatorPin MuleMicrocontrollerSimulatorPlatform::internal_readPin(int pn) {
	MuleMicrocontrollerSimulatorPin toret;
	toret.num = pn;
	MULE_OTHER_STRINGTYPE possiblePinPath = MULE_OTHER_STRINGTYPE(MULE_SIMDIRECTORY) + "/PIN" + internal_muleIntToStr(pn);
	toret.path = possiblePinPath;
	MULE_OTHER_STRINGTYPE rd = internal_readToString(possiblePinPath);
	MULE_OTHER_STRINGTYPE rdconv = rd;
	std::vector<MULE_OTHER_STRINGTYPE> splitvec;
	MULE_OTHER_STRINGTYPE topb = "";
	muledebug("loop through pbconv begin");
	for (int i = 0; i < rdconv.size(); i++) {
		muledebug("i = " + muleinttostr(i));
		if (rdconv[i] == ',') {
			splitvec.push_back(topb);
			topb = "";
		}
		else
			topb = topb + rdconv[i];
	}
	splitvec.push_back(topb);
	muledebug("loop through pbconv end");
	
	//delete rdconv;
	for (int j = 0; j < splitvec.size(); j++) {
		MULE_OTHER_STRINGTYPE firsthalf = "";
		MULE_OTHER_STRINGTYPE secondhalf = "";
		MULE_OTHER_STRINGTYPE curitem = splitvec.at(j);
		int end = curitem.size();
		int stopgap = curitem.find('=', 0);
		if (stopgap == MULE_OTHER_STRINGTYPE::npos)
			stopgap = end;
		muledebug("stopgap = " + muleinttostr(stopgap));
		int lastpos = 0;
		for (int k = 0; k < stopgap; k++) {
			firsthalf = firsthalf + curitem[k];
			lastpos = k;
			muledebug("k = " + muleinttostr(k));
		}
		lastpos = lastpos + 2;
		if (lastpos > end)
			lastpos = (int)(end / 2);
		for (int l = lastpos; l < end; l++)
			secondhalf = secondhalf + curitem[l];
		//delete curitem;
		if (secondhalf == "U" || secondhalf == "u")
			secondhalf = "0";
		if (firsthalf == "type")
			toret.digital = (secondhalf == "d");
		else if (firsthalf == "pud") {
			if (secondhalf == "off")
				toret.pud = MULE_PUD_OFF;
			else if (secondhalf == "down")
				toret.pud = MULE_PUD_DOWN;
			else
				toret.pud = MULE_PUD_UP;
		}
		else if (firsthalf == "pwm") {
			if (secondhalf == "no")
				toret.pwm = true;
			else
				toret.pwm = false;
		}
		else if (firsthalf == "inout") {
			if (secondhalf == "i")
				toret.input = true;
			else
				toret.input = false;
		}
		else if (firsthalf == "val")
			toret.val = std::atoi(secondhalf.c_str());
		else if (firsthalf == "dc")
			toret.dc = std::atoi(secondhalf.c_str());
		else if (firsthalf == "maxdc")
			toret.maxdc = std::atoi(secondhalf.c_str());
		else if (firsthalf == "freq")
			toret.freq = std::atoi(secondhalf.c_str());
	}
	return toret;
	
}

MULE_OTHER_STRINGTYPE MuleMicrocontrollerSimulatorPlatform::internal_readToString(MULE_OTHER_STRINGTYPE fn) {
	std::ifstream stream(fn.c_str());
#ifndef __sun
	return MULE_OTHER_STRINGTYPE((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
#else
	std::stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
#endif
}

bool MuleMicrocontrollerSimulatorPlatform::internal_fileExists(MULE_OTHER_STRINGTYPE fn) {
	std::ifstream* stream = new std::ifstream(fn.c_str());
	bool toret = stream->good();
	delete stream;
	return toret;
}

bool MuleMicrocontrollerSimulatorPlatform::initialize() {
    if (internal_fileExists(MULE_OTHER_STRINGTYPE(MULE_SIMDIRECTORY) + "/PIN0") == false)
	return false;
    devlist.clear();
    for (int i = 0; i < 4; i++)
	devlist.push_back(new MuleDevice(i));
    return true;
}

#ifdef MULE_FEATURES_CORE
std::vector<MuleDevice*> MuleMicrocontrollerSimulatorPlatform::getDevices() {
    return devlist;
}

MULE_OTHER_HWPINTYPE MuleMicrocontrollerSimulatorPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    if (internal_readPin(pin).input == true)
	return MULE_INPUT;
    return MULE_OUTPUT;
}

bool MuleMicrocontrollerSimulatorPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("mode = " + muleinttostr((int)(mode)));
    MuleMicrocontrollerSimulatorPin pin_s = internal_readPin(pin);
    pin_s.input = (mode == MULE_INPUT);
    return internal_flushPin(pin_s);
}

MULE_OTHER_HWPINTYPE MuleMicrocontrollerSimulatorPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    return internal_readPin(pin).val;
}

bool MuleMicrocontrollerSimulatorPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("ct = " + muleinttostr((int)(ct)));
    MuleMicrocontrollerSimulatorPin pin_s = internal_readPin(pin);
    pin_s.val = ct;
    return internal_flushPin(pin_s);
}

bool MuleMicrocontrollerSimulatorPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    muledebug("pin = " + muleinttostr((int)(pin)));
    muledebug("val = " + muleinttostr((int)(val)));
    MuleMicrocontrollerSimulatorPin pin_s = internal_readPin(pin);
    pin_s.pud = val;
    return internal_flushPin(pin_s);
}
#endif


#ifdef MULE_FEATURES_PWMDEVICES
bool MuleMicrocontrollerSimulatorPlatform::startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) {
	MuleMicrocontrollerSimulatorPin pin_s = internal_readPin(pin);
	pin_s.dc = dutycycle;
	return internal_flushPin(pin_s);
}

MULE_OTHER_HWPINTYPE MuleMicrocontrollerSimulatorPlatform::getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) {
	return internal_readPin(pin).dc;
}

MULE_OTHER_HWPINTYPE MuleMicrocontrollerSimulatorPlatform::getPWMRange(MULE_OTHER_HWPINTYPE pin) {
	return internal_readPin(pin).maxdc;
}

bool MuleMicrocontrollerSimulatorPlatform::setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) {
	MuleMicrocontrollerSimulatorPin pin_s = internal_readPin(pin);
	pin_s.maxdc = range;
	return internal_flushPin(pin_s);
}

MULE_OTHER_HWPINTYPE MuleMicrocontrollerSimulatorPlatform::getPWMFrequency(MULE_OTHER_HWPINTYPE pin) {
	return internal_readPin(pin).freq;
}

bool MuleMicrocontrollerSimulatorPlatform::setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) {
	MuleMicrocontrollerSimulatorPin pin_s = internal_readPin(pin);
	pin_s.freq = freq;
	return internal_flushPin(pin_s);
}
#endif

#ifdef MULE_FEATURES_SENSORS
bool MuleMicrocontrollerSimulatorPlatform::sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) {
	int orval = internal_readPin(pin).val;
	while (orval == internal_readPin(pin).val)
		muledebug("waiting for trigger");
	return true;
}
#endif
