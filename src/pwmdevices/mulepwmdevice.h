#ifndef MULEPWMDEVICE_H
#define MULEPWMDEVICE_H

#include "core/muledevice.h"
#include "core/muleglobalfunctions.h"
#include "platformsupport/common/mulecurrentplatform.h"

class MulePWMDevice : public MuleDevice {
	public:
	  MulePWMDevice(int devpin);
	  ~MulePWMDevice();
	  bool start(MULE_OTHER_HWPINTYPE dutycycle);
	  bool on() { return this->start(getRange()); }
	  bool stop() { return this->start(0); }
	  bool off() { return this->stop(); }
	  int getRange();
	  int range() { return this->getRange(); }
	  int getDutyCycle();
	  int dutyCycle() { return this->getDutyCycle(); }
	  int getFrequency();
	  int frequency() { return this->getFrequency(); }
	  bool isOn();
	  bool isActive() { return this->isOn(); };
	  bool setRange(MULE_OTHER_HWPINTYPE range);
	  bool setFrequency(MULE_OTHER_HWPINTYPE frequency);
	  
	private:
	  bool deviceIsTurnedOn;
};

#endif
