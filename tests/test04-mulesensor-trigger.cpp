#include <mule.h>

#define SENSORPIN 2

int main() {
	MuleApplication mApp;
	muleprintf("MuleSensor test on %s", mApp.getPlatformName().c_str());
	muleprintf("Step 1. Create a new instance of MuleSensor... ");
	MuleSensor lsensor(SENSORPIN);
	muleprintf("success\n");
	muleprintf("Step 2. Wait till triggered... ");
	if (lsensor.waitUntilTriggered() == false) {
		muleprintf("failed\n");
		mApp.exit(1);
	}
	else
		muleprintf("success\n");
	muleprintf("That's it, goodbye\n");
	mApp.exit(0);
	return 0;
}

