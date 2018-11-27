#ifndef MULEMULTIPINDEVICE_H
#define MULEMULTIPINDEVICE_H

#define MULE_MULTIPIN_LIMIT 8

#include <vector>
#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"
#include "core/muledevice.h"

class MuleMultipinDevice {
	public:
	  MuleMultipinDevice(MULE_OTHER_HWPINTYPE pin1, MULE_OTHER_HWPINTYPE pin2 = -1, MULE_OTHER_HWPINTYPE pin3 = -1, MULE_OTHER_HWPINTYPE pin4 = -1, MULE_OTHER_HWPINTYPE pin5 = -1, MULE_OTHER_HWPINTYPE pin6 = -1, MULE_OTHER_HWPINTYPE pin7 = -1, MULE_OTHER_HWPINTYPE pin8 = -1);
	  MuleMultipinDevice(MuleDevice dev1, MuleDevice dev2 = MuleDevice(-1), MuleDevice dev3 = MuleDevice(-1), MuleDevice dev4 = MuleDevice(-1), MuleDevice dev5 = MuleDevice(-1), MuleDevice dev6 = MuleDevice(-1), MuleDevice dev7 = MuleDevice(-1), MuleDevice dev8 = MuleDevice(-1));
	  MuleMultipinDevice(MuleDevice* dev1, MuleDevice* dev2 = new MuleDevice(-1), MuleDevice* dev3 = new MuleDevice(-1), MuleDevice* dev4 = new MuleDevice(-1), MuleDevice* dev5 = new MuleDevice(-1), MuleDevice* dev6 = new MuleDevice(-1), MuleDevice* dev7 = new MuleDevice(-1), MuleDevice* dev8 = new MuleDevice(-1));
	  MuleMultipinDevice(std::vector<MuleDevice> devs);
	  MuleMultipinDevice(std::vector<MuleDevice*> devs);
	  MuleMultipinDevice(int pins[MULE_MULTIPIN_LIMIT]);
	  MuleMultipinDevice(std::vector<MULE_OTHER_HWPINTYPE> pinsvec);
	  ~MuleMultipinDevice();
	  
	  MuleDevice* getPin(int nPin);
	  MuleDevice* pin(int nPin) { return this->getPin(nPin); }
	  MuleDevice* getIndex(int nIndex);
	  MuleDevice* index(int nIndex) { return this->getIndex(nIndex); }
	  int count() { return mDevices.size(); }
	  int getCount() { return this->count(); }
	  bool removePin(int nPin);
	  bool removeIndex(int nIndex);
	  bool erasePin(int nPin) { return this->removePin(nPin); }
	  bool eraseIndex(int nIndex) { return this->removeIndex(nIndex); }
	  bool addPin(MULE_OTHER_HWPINTYPE nPin) { return this->addPin(new MuleDevice(nPin)); }
	  bool addPin(MuleDevice dev) { return this->addPin(&dev); }
	  bool addPin(MuleDevice* dev);
	  bool writeToAll(MULE_OTHER_HWPINTYPE val);
	  bool write(MULE_OTHER_HWPINTYPE val) { return this->writeToAll(val); }
	  std::vector<MULE_OTHER_HWPINTYPE> readFromAll();
	  std::vector<MULE_OTHER_HWPINTYPE> read() { return this->readFromAll(); }
	  
	  std::vector<MULE_OTHER_HWPINTYPE> toPinVector();
	  std::vector<MuleDevice*> toDevicePointerVector() { return mDevices; }
	  std::vector<MULE_OTHER_HWPINTYPE> getPinVector() { return this->toPinVector(); }
	  std::vector<MuleDevice*> getDevicePointerVector() { return this->toDevicePointerVector(); }
	  
	protected:
	  std::vector<MuleDevice*> mDevices;
	  
	  int internalInit(MULE_OTHER_HWPINTYPE pin1, MULE_OTHER_HWPINTYPE pin2 = -1, MULE_OTHER_HWPINTYPE pin3 = -1, MULE_OTHER_HWPINTYPE pin4 = -1, MULE_OTHER_HWPINTYPE pin5 = -1, MULE_OTHER_HWPINTYPE pin6 = -1, MULE_OTHER_HWPINTYPE pin7 = -1, MULE_OTHER_HWPINTYPE pin8 = -1);
	  int internalInit(MuleDevice dev1, MuleDevice dev2 = MuleDevice(-1), MuleDevice dev3 = MuleDevice(-1), MuleDevice dev4 = MuleDevice(-1), MuleDevice dev5 = MuleDevice(-1), MuleDevice dev6 = MuleDevice(-1), MuleDevice dev7 = MuleDevice(-1), MuleDevice dev8 = MuleDevice(-1));
	  int internalInit(std::vector<MuleDevice> devs);
	  int internalInit(std::vector<MuleDevice*> devs);
	  int internalInit(int pins[MULE_MULTIPIN_LIMIT]);
	  int internalInit(std::vector<MULE_OTHER_HWPINTYPE> pinsvec);
	  int internalInit(MuleDevice* dev1, MuleDevice* dev2 = new MuleDevice(-1), MuleDevice* dev3 = new MuleDevice(-1), MuleDevice* dev4 = new MuleDevice(-1), MuleDevice* dev5 = new MuleDevice(-1), MuleDevice* dev6 = new MuleDevice(-1), MuleDevice* dev7 = new MuleDevice(-1), MuleDevice* dev8 = new MuleDevice(-1));
	
};

#endif
