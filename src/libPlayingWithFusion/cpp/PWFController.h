#pragma once
#include <stdint.h>


typedef enum {
   DEV_TOF_SENSOR = 0x110,
   DEV_TOF_SENSOR_CK128 = 0x111,
   DEV_TOF_SENSOR_CK256 = 0x112,
   DEV_VENOM_MOTOR_CONTROLLER_PROTO = 0x123,
   DEV_VENOM_MOTOR_CONTROLLER = 0x124,
   DEV_ANALOG_TOF_SENSOR_CK128 = 0x311,
   DEV_BFG_SENSOR = 0x130
} pwf_device_t;


class PWFController {
public:
   pwf_device_t device;
   uint32_t serialNumber;
   uint32_t firmwareVersion;
   uint32_t latestFwVersion;
   uint16_t timeSinceLastHeartbeat;
   uint8_t activeOnBus;
   uint8_t id;


   bool operator ==(const PWFController & obj) const {
      if ((device == obj.device) && (serialNumber == obj.serialNumber))
         return true;
      else
         return false;
   }
};