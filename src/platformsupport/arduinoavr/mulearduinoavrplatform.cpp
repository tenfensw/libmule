#include "platformsupport/arduinoavr/mulearduinoavrplatform.h"

MuleArduinoAVRPlatform::MuleArduinoAVRPlatform() {
      // TODO: implement device initialization code here
}

std::vector<MuleDevice*> MuleArduinoAVRPlatform::getDevices() {
     std::vector<MuleDevice*> list_of_connected_devices;
     list_of_connected_devices.clear();
     int maxpincount = 54; // replace this with the total amount of GPIO/device pins available on your device
     for (int i = 0; i < maxpincount; i++)
          list_of_connected_devices.push_back(new MuleDevice(i));
     return list_of_connected_devices;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPinMode(MULE_OTHER_HWPINTYPE pin) {
     // TODO: implement this function.
     // the only thing it does is checking if the specified pin is an input or an output pin and returning either MULE_INPUT or MULE_OUTPUT
}

bool MuleArduinoAVRPlatform::setPinMode(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE mode) {
     // TODO: implement this function.
     // the only thing it does is initializing the specified pin either as an input or an output pin (if everything is okay, then true is returned, if not, then false is returned)
     return false;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::readFromPin(MULE_OTHER_HWPINTYPE pin) {
    // TODO: implement this function.
    // it gets a value from the sensor connected to the specified pin and returns that value
    return -1;
}

bool MuleArduinoAVRPlatform::writeToPin(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE ct) {
    // TODO: implement this function.
    // it tries to write "ct" to the specified pin
    return false;
}

bool MuleArduinoAVRPlatform::setPullUpDown(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE val) {
    // TODO: implement this function IF your target device actually supports pull-up-down resistors.
}

#ifdef MULE_FEATURES_PWMDEVICES
bool MuleArduinoAVRPlatform::startPWM(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE dutycycle) {
    // TODO: implement this function
    // it sends PWM pulses to the specified pin
    return false;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPWMDutyCycle(MULE_OTHER_HWPINTYPE pin) {
    // TODO: implement this function
    // it returns the duty cycle setting on the specified pin
    return -1;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPWMRange(MULE_OTHER_HWPINTYPE pin) {
    // TODO: implement this function
    // it returns the configured PWM range on the specified pin
    return -1;
}

MULE_OTHER_HWPINTYPE MuleArduinoAVRPlatform::getPWMFrequency(MULE_OTHER_HWPINTYPE pin) {
    // TODO: implement this function
    // it returns the PWM frequency on the specified pin
    return -1;
}

bool MuleArduinoAVRPlatform::setPWMRange(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE range) {
    // TODO: implement this function
    // it sets the maximum PWM range on the specified pin
    return false;
}

bool MuleArduinoAVRPlatform::setPWMFrequency(MULE_OTHER_HWPINTYPE pin, MULE_OTHER_HWPINTYPE freq) {
    // TODO: implement this function
    // it sets the PWM frequency of the specified pin
    return false;
}

#endif
#ifdef MULE_FEATURES_SENSORS
bool MuleArduinoAVRPlatform::sensorWaitUntilTriggered(MULE_OTHER_HWPINTYPE pin) {
    // TODO: implement this function
    // it just waits until the specified pin goes from low to high and returns true
    return false;
}
#endif

