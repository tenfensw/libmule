#ifndef MULEMOTORDEVICE_H
#define MULEMOTORDEVICE_H

#include "core/muledevice.h"

class MuleMotorDevice : public MuleDevice {
	public:
	  MuleMotorDevice(int devpin);
	  bool go(int speed);
	  void stop();
	  	
};

#endif
