#ifndef MULEMOTORDEVICE_H
#define MULEMOTORDEVICE_H

#include "core/muledevice.h"
#include "core/muleglobalfunctions.h"

class MuleMotorDevice : public MuleDevice {
	public:
	  MuleMotorDevice(int devpin);
	  bool goForward(int speed);
	  bool goBack(int speed);
	  void stop();
	  	
};

#endif
