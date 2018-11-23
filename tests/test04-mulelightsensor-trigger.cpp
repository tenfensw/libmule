#include <mule.h>

#define SENSORPIN 2

int main() {
	MuleApplication mApp;
	muleprintf("MuleLightSensor test on %s", mApp.getPlatformName().c_str());
	muleprintf("Step 1. Create a new instance of MuleLightSensor");
	MuleLightSensor lsensor(SENSORPIN);
	muleprintf("success");
	muleprintf("Step 2. Wait till triggered");
	if (lsensor.waitUntilTriggered() == false) {
		muleprintf("failed");
		mApp.exit(1);
	}
	else
		muleprintf("success");
	muleprintf("That's it, goodbye");
	mApp.exit(0);
	return 0;
}
