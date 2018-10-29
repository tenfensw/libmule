// Testing if MuleApplication works correctly
#include <mule.h>
#include <iostream>

int main() {
	MuleApplication mApp;
	std::cout << "Current platform is " << mApp.getPlatformName() << std::endl;
	std::cout << "Current directory is " << mApp.getCurrentDirectory() << std::endl;
	std::cout << "Goodbye" << std::endl;
	mApp.exit(0);
	return 0;
}
