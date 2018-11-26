#include "core/mulemultipindevice.h"

MuleMultipinDevice::MuleMultipinDevice(MULE_OTHER_HWPINTYPE pin1, MULE_OTHER_HWPINTYPE pin2, MULE_OTHER_HWPINTYPE pin3, MULE_OTHER_HWPINTYPE pin4, MULE_OTHER_HWPINTYPE pin5, MULE_OTHER_HWPINTYPE pin6, MULE_OTHER_HWPINTYPE pin7, MULE_OTHER_HWPINTYPE pin8) {
	MuleMultipinDevice(MuleDevice(pin1), MuleDevice(pin2), MuleDevice(pin3), MuleDevice(pin4), MuleDevice(pin5), MuleDevice(pin6), MuleDevice(pin7), MuleDevice(pin8));
}

MuleMultipinDevice::MuleMultipinDevice(const MuleDevice& dev1, const MuleDevice& dev2, const MuleDevice& dev3, const MuleDevice& dev4, const MuleDevice& dev5, const MuleDevice& dev5, const MuleDevice& dev6, const MuleDevice& dev7, const MuleDevice& dev8) {
	std::vector<MuleDevice*> devices;
	devices.push_back(&dev1);
	if (dev2.getPin() != -1)
		devices.push_back(&dev2);
	if (dev3.getPin() != -1)
		devices.push_back(&dev3);
	if (dev4.getPin() != -1)
		devices.push_back(&dev4);
	if (dev5.getPin() != -1)
		devices.push_back(&dev5);
	if (dev6.getPin() != -1)
		devices.push_back(&dev6);
	if (dev7.getPin() != -1)
		devices.push_back(&dev7);
	if (dev8.getPin() != -1)
		devices.push_back(&dev8);
	mDevices = devices;
}

MuleMultipinDevice::MuleMultipinDevice(const std::vector<MuleDevice>& devs) {
	if (devs.size() > MULE_MULTIPIN_LIMIT)
		muleexception(-24, "MuleMultipinDevice can maximum handle " + muleinttostr(MULE_MULTIPIN_LIMIT) + " devices", false);
	std::vector<MuleDevice*> devices;
	for (int i = 0; i < devs.size(); i++) {
		if (devices[i].getPin() != -1)
			devices.push_back(&(devices[i]));
	}
	mDevices = devices;
}

MuleMultipinDevice::MuleMultipinDevice(const std::vector<MuleDevice*>& devs) {
	if (devs.size() > MULE_MULTIPIN_LIMIT)
		muleexception(-24, "MuleMultipinDevice can maximum handle " + muleinttostr(MULE_MULTIPIN_LIMIT) + " devices", false);
	mDevices = devs;
}

MuleMultipinDevice::MuleMultipinDevice(int pins[MULE_MULTIPIN_LIMIT]) {
	MuleMultipinDevice(pins[0], pins[1], pins[2], pins[3], pins[4], pins[5], pins[6], pins[7]);
}

MuleMultipinDevice::MuleMultipinDevice(const std::vector<MULE_OTHER_HWPINTYPE>& pinsvec) {
	if (pinsvec.size() > MULE_MULTIPIN_LIMIT)
		muleexception(-24, "MuleMultipinDevice can maximum handle " + muleinttostr(MULE_MULTIPIN_LIMIT) + " devices", false);
	std::vector<MuleDevice*> devvec;
	for (int i = 0; i < pinsvec.size(); i++) {
		if (pinsvec[i] != -1)
			devvec.push_back(new MuleDevice(pinsvec[i]));
	}
	mDevices = devvec;
}

MuleMultipinDevice::~MuleMultipinDevice() {
	for (int i = 0; i < mDevices.size(); i++)
		delete mDevices[i];
}

MuleDevice* MuleMultipinDevice::getPin(int nPin) {
	for (int i = 0; i < mDevices.size(); i++) {
		if (mDevices[i]->getPin() == nPin)
			return mDevices[i];
	}
	muleexception(-25, "No device with pin " + muleinttostr(nPin) + " found");
	return new MuleDevice(-1);
}

MuleDevice* MuleMultipinDevice::getIndex(int nIndex) {
	if (nIndex > (mDevices.size() - 1))
		return mDevices[nIndex];
	rmuleexception(-26, "No device with index " + muleinttostr(nIndex) + " found");
	return new MuleDevice(-1);
}

bool MuleMultipinDevice::removePin(int nPin) {
	for (int i = 0; i < mDevices.size(); i++) {
		if (mDevices[i]->getPin() == nPin) {
			mDevices.erase(mDevices.begin() + i);
			return true;
		}
	}
	muleexception(-25, "No device with pin " + muleinttostr(nPin) + " found");
	return false;
}

bool MuleMultipinDevice::removeIndex(int nIndex) {
	if (nIndex < mDevices.size()) {
		mDevices.erase(mDevices.begin() + nIndex);
		return true;
	}
	muleexception(-26, "No device with index " + muleinttostr(nIndex) + " found");
	return false;
}

bool MuleMultipinDevice::addPin(MuleDevice* dev) {
	if ((mDevices.size() + 1) > MULE_MULTIPIN_LIMIT) {
		muleexception(-24, "MuleMultipinDevice can maximum handle " + muleinttostr(MULE_MULTIPIN_LIMIT) + " devices");
		return false;
	}
	mDevices.push_back(dev);
	return true;
}

bool MuleMultipinDevice::writeToAll(MULE_OTHER_HWPINTYPE val) {
	 for (int i = 0; i < mDevices.size(); i++) {
		if (mDevices[i]->write(val) == false) {
			muleexception(-28, "Cannot write to device");
			return false;
		}
	 }
	 return false;
}

std::vector<MULE_OTHER_HWPINTYPE> MuleMultipinDevice::readFromAll() {
	std::vector<MULE_OTHER_HWPINTYPE> resultvec;
	for (int i = 0; i < mDevices.size(); i++)
		resultvec.push_back(mDevices[i]->read());
	return resultvec;
}

std::vector<MULE_OTHER_HWPINTYPE> MuleMultipinDevice::toPinVector() {
	std::vector<MULE_OTHER_HWPINTYPE> resultvec;
	for (int i = 0; i < mDevices.size(); i++)
		resultvec.push_back(mDevices[i]->getPin());
	return resultvec;
}