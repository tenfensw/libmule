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

#include "platformsupport/arduinoavr/mulearduinoavrplatform.h"

MuleArduinoAVRPlatform::MuleArduinoAVRPlatform() {
      init();
#ifndef MULE_ARDUINO_OBSOLETESDK
      initVariant();
#endif
      delay(1);
#if defined(USBCON)
      USBDevice.attach();
#endif
     std::vector<MuleDevice*> list_of_connected_devices;
     list_of_connected_devices.clear();
     int maxpincount = MULE_ARDUINO_DIGITALPINCOUNT + MULE_ARDUINO_ANALOGPINCOUNT; 
     for (int i = 0; i < maxpincount; i++)
          list_of_connected_devices.push_back(new MuleDevice(i));
     devlist = list_of_connected_devices;
}

void MuleArduinoAVRPlatform::internal_cleanDevList() {
    for (int i = 0; i < devlist.size(); i++) {
	delete devlist.at(i);
	devlist.at(i) = nullptr;
    }
    devlist.clear();
}

bool MuleArduinoAVRPlatform::arduinoIsDigitalPin(MULE_OTHER_HWPINTYPE pin) {
	if (pin < (MULE_ARDUINO_LASTDIGITALPIN + 1))
		return true;
	return false;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::arduinoMulePinToNativeAnalog(MULE_OTHER_HWPINTYPE pin) {
	if (arduinoIsDigitalPin(pin) == true)
		return -1;
	return (pin - MULE_ARDUINO_LASTDIGITALPIN - 1);
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
	if (arduinoIsDigitalPin(pin) == false)
		return MULE_INPUT;
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t prt = digitalPinToPort(pin);
	volatile uint8_t *pinregister = portModeRegister(prt);
	
	if (*pinregister & bit)
		return MULE_OUTPUT;
	else
		return MULE_INPUT;
		
	return -1;
}

bool MuleArduinoAVRPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
	if (arduinoIsDigitalPin(pin) == false)
		return false;
	pinMode(pin, mode);
	return true;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    if (arduinoIsDigitalPin(pin) == true)
	return digitalRead(pin);
    else {
	int arduinoanalog = arduinoMulePinToNativeAnalog(pin);
	if (pin > -1)
		return analogRead(arduinoanalog);
    }
    return -1;
}

bool MuleArduinoAVRPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    if (arduinoIsDigitalPin(pin) == true)
	digitalWrite(pin, ct);
    else
	return false;
    return true;
}

bool MuleArduinoAVRPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    if (val != MULE_PUD_OFF && val != MULE_PUD_DOWN && val != MULE_PUD_UP)
	return false;
    return setPinMode(pin, val);
}

#ifdef MULE_FEATURES_PWMDEVICES
bool MuleArduinoAVRPlatform::startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) {
    if (dutycycle > MULE_ARDUINO_MAXDUTYCYCLE || dutycycle < MULE_ARDUINO_MINDUTYCYCLE)
	return false;
    int analogpin = pin;
    if (arduinoIsDigitalPin(pin) == false)
	analogpin = arduinoMulePinToNativeAnalog(pin);
    analogWrite(analogpin, dutycycle);
    return true;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) {
    return MULE_ARDUINO_MAXDUTYCYCLE;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPWMRange(MULE_OTHER_HWPINTYPE pin) {
    return getPWMDutyCycle(pin);
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPWMFrequency(MULE_OTHER_HWPINTYPE pin) {
    return MULE_ARDUINO_CONSTPWMFREQUENCY;
}

bool MuleArduinoAVRPlatform::setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) {
    return false;
}

bool MuleArduinoAVRPlatform::setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) {
    return false;
}

#endif
#ifdef MULE_FEATURES_SENSORS
bool MuleArduinoAVRPlatform::sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) {
    return false;
}
#endif

