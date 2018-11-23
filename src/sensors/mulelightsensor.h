#ifndef MULELIGHTSENSOR_H
#define MULELIGHTSENSOR_H

#include "platformsupport/common/mulecurrentplatform.h"
#include "core/muledevice.h"


class MuleLightSensor : public MuleDevice {
	public:
	  MuleLightSensor(int devpin);
	  bool waitUntilTriggered();

};
#endif
