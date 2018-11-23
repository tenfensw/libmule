#ifndef MULESERVODEVICE_H
#define MULESERVODEVICE_H
#include "pwmdevices/mulepwmdevice.h"

class MuleServoDevice : public MulePWMDevice {
	public:
	  MuleServoDevice(int devpin);
	  ~MuleServoDevice();
	  bool rotate(int angle = 90);
	  bool goBackAndForth(int startangle = 0, int endangle = 90, int times = 1, double delay = 0.5);
};

#endif
