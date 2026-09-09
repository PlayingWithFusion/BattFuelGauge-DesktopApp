// FusionFlashUtil.cpp : Defines the entry point for the console application.
//

#include<iostream> //HACK

#include <thread>
#include <mutex>
#include <stdio.h>
#include "FusionFlashUtil.h"
#include <list>
#include <chrono>
#include <stdexcept>
#include <fmt/format.h>
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
#include <FRC_NetworkCommunication/CANSessionMux.h>
#endif
#include <wpi/raw_ostream.h>

using namespace pwf;


static inline uint32_t Min(uint32_t x, uint32_t y)
{
   return (x < y) ? x : y;
}

FusionFlashUtil::FusionFlashUtil() {
   m_pIdentifyThread = new std::thread(FusionFlashUtil::CallIdentifyHeartbeatTask, this);
}

FusionFlashUtil::~FusionFlashUtil() {
   m_terminateThreads = true;
   m_pIdentifyThread->join();

   if (m_pFlashThread) {
      m_pFlashThread->join();
   }
}

void FusionFlashUtil::CallIdentifyHeartbeatTask(void *pFlashUtil) {
   FusionFlashUtil *flashUtil = (FusionFlashUtil*)pFlashUtil;
   flashUtil->IdentifyHeartbeatTask();
}


void FusionFlashUtil::IdentifyHeartbeatTask() {
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
#define RX_BUF_LEN   16
   const uint32_t msgIDs[] = { 0x1F0501FF, 0x1F0B0100 /*0x18F0FF00*/ };
   struct tCANStreamMessage msgList[RX_BUF_LEN];
   uint32_t sessionHandle;
   int32_t status;
   uint32_t messagesRead;
   PWFController controller;
   uint32_t mask = ~(msgIDs[0] ^ msgIDs[1]);

   FRC_NetworkCommunication_CANSessionMux_openStreamSession(&sessionHandle, msgIDs[0]&mask, mask, RX_BUF_LEN, &status);

   while (m_terminateThreads == false) {
      FRC_NetworkCommunication_CANSessionMux_readStreamSession(sessionHandle, &msgList[0], RX_BUF_LEN, &messagesRead, &status);
      
      for (uint32_t i = 0; i < messagesRead; i++) {
         if ( (msgList[i].dataSize == 8) &&
              (((msgList[i].messageID&0x1FFFFF00) == msgIDs[0]) || ((msgList[i].messageID&0x1FFFFF00) == msgIDs[1])) ) {
            controller.id = (uint8_t)msgList[i].messageID & 0xFF;

            controller.device = (pwf_device_t)(((uint32_t)msgList[i].data[5] << 8) | (uint32_t)msgList[i].data[4]);

            controller.serialNumber = ((uint32_t)msgList[i].data[3] << 16) |
                                      ((uint32_t)msgList[i].data[2] << 8) |
                                      (uint32_t)msgList[i].data[1];

            controller.firmwareVersion = ((uint32_t)msgList[i].data[7] << 8) | (uint32_t)msgList[i].data[6];

            controller.timeSinceLastHeartbeat = 0;

            std::string deviceName = fmt::format("0x{:X}", (int)controller.device);
            if (controller.device == pwf_device_t::DEV_VENOM_MOTOR_CONTROLLER) {
               controller.latestFwVersion = 0;
               deviceName = "Venom Motor";
            }
            else if (controller.device == pwf_device_t::DEV_VENOM_MOTOR_CONTROLLER_PROTO) {
               controller.latestFwVersion = 0;
               deviceName = "Venom Motor";
            }
            else if (controller.device == pwf_device_t::DEV_TOF_SENSOR) {
               controller.latestFwVersion = 0;
               deviceName = "Time-of-Flight";
            }
            else if ((controller.device == pwf_device_t::DEV_TOF_SENSOR_CK128) ||
                     (controller.device == pwf_device_t::DEV_ANALOG_TOF_SENSOR_CK128)) {
               controller.latestFwVersion = 0;
               deviceName = "Time-of-Flight";
            }
            else if (controller.device == pwf_device_t::DEV_TOF_SENSOR_CK256) {
               controller.latestFwVersion = 0;
               deviceName = "Time-of-Flight";
            }
            else if (controller.device == pwf_device_t::DEV_BFG_SENSOR) {
               controller.latestFwVersion = 0;
               deviceName = "Battery Fuel Gauge";
            }

            // Add to the controllers list if this 'lil guy is new around here
            bool foundController = false;
            for (auto itr = m_controllerList.begin(); itr != m_controllerList.end(); itr++) {
               if (*itr == controller) {
                  foundController = true;
                  *itr = controller;
                  break;
               }
            }
            
            if (!foundController) {
               m_controllerList.push_back(controller);
               wpi::errs() << "Found PWF Device: " << deviceName << ", serial number:" << std::to_string(controller.serialNumber) << "\n";
            }
         }
      }

      if (messagesRead == 0)
      {
         // Loop through all the known controllers and mark them as 'active' if we've received a heartbeat
         // CAN message in the last 4 seconds (40 ticks).
         for (auto itr = m_controllerList.begin(); itr != m_controllerList.end(); itr++) {
            const uint16_t heartbeatTimeout = 40;
            if (itr->timeSinceLastHeartbeat < heartbeatTimeout)
            {
               itr->activeOnBus = 1;
               itr->timeSinceLastHeartbeat++;
            }
            else
            {
               itr->activeOnBus = 0;
            }
         }

         std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
   }

   FRC_NetworkCommunication_CANSessionMux_closeStreamSession(sessionHandle);
#endif   
}

std::list<PWFController> FusionFlashUtil::GetControllerList() {
   return m_controllerList;
}

PWFController * FusionFlashUtil::GetControllerById(pwf_device_t device, uint8_t id)
{
   PWFController *pController = NULL;

   for (auto itr = m_controllerList.begin(); itr != m_controllerList.end(); itr++)
   {
      if ((itr->device == device) && (itr->id == id) && (itr->activeOnBus))
      {
         pController = &(*itr);
         break;
      }
   }

   return pController;
}

void FusionFlashUtil::SetControllerId(pwf_device_t device, uint32_t serialNumber, uint8_t id)
{
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   for (auto controller = m_controllerList.begin(); controller != m_controllerList.end(); controller++) {
      if ((controller->device == device) && (controller->serialNumber == serialNumber)) {
         controller->id = id;

         uint8_t data[8];
         int32_t status;

         data[0] = 0x0C;
         data[1] = (serialNumber & 0xFF);
         data[2] = (serialNumber >> 8);
         data[3] = (serialNumber >> 16);
         data[4] = (device & 0xFF);
         data[5] = (device >> 8);
         data[6] = id;

         FRC_NetworkCommunication_CANSessionMux_sendMessage(0x1F0B03FF, &data[0], 7, 0, &status);
         break;
      }
   }
#endif
}

void FusionFlashUtil::IdentifyController(pwf_device_t device, uint32_t serialNumber)
{
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   uint8_t data[8];
   int32_t status;

   data[0] = 0x0D;
   data[1] = (serialNumber & 0xFF);
   data[2] = (serialNumber >> 8);
   data[3] = (serialNumber >> 16);
   data[4] = (device & 0xFF);
   data[5] = (device >> 8);

   FRC_NetworkCommunication_CANSessionMux_sendMessage(0x1F0B03FF, &data[0], 6, 0, &status);
#endif
}

void FusionFlashUtil::UnlockController(pwf_device_t device, uint32_t serialNumber)
{
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   uint8_t data[8];
   int32_t status;

   data[0] = 0x0A;  //Unlock Command
   data[1] = (serialNumber & 0xFF);
   data[2] = (serialNumber >> 8);
   data[3] = (serialNumber >> 16);
   data[4] = (device & 0xFF);
   data[5] = (device >> 8);

   FRC_NetworkCommunication_CANSessionMux_sendMessage(0x1F0B03FF, &data[0], 6, 0, &status);
#endif
}


void FusionFlashUtil::BeginFlash(pwf_device_t device, uint32_t serialNumber) {
   m_deviceID = device;
   m_serialNumber = serialNumber;
}

FlashingState FusionFlashUtil::GetFlashState() {
   return m_state;
}
unsigned int FusionFlashUtil::GetFlashPercentComplete() {
   return m_percentComplete;
}

std::string FusionFlashUtil::GetLastFlashError() {
//   return m_errorString;
   return NULL;
}

void FusionFlashUtil::CallFlashTask(void *pFlashUtil) {
}

void FusionFlashUtil::FlashTask() {
}
