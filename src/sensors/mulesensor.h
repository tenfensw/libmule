#ifndef MULESENSOR_H
#define MULESENSOR_H

#include "platformsupport/common/mulecurrentplatform.h"
#include "core/muledevice.h"


class MuleSensor : public MuleDevice {
	public:
	  MuleSensor(int devpin);
	  ~MuleSensor();
	  bool waitUntilTriggered();

};
#endif
