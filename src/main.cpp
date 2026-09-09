// libPlayingWithFusionDriver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>
#include <chrono>
#include <unordered_map>
#include <queue>
#include <fmt/format.h>
#include "libPlayingWithFusion/cpp/FusionFlashUtil.h"
#include "libPlayingWithFusion/cpp/FusionHttpServer.h"
#include "FRC_NetworkCommunication\FRCComm.h"
#include "hal/DriverStation.h"
#include "hal/HALBase.h"

#include "halCAN.h"
#include <windows.h>

pwf::FusionFlashUtil flashUtil;
pwf::FusionHttpServer httpServer(flashUtil);


int32_t HAL_GetTeamNumber(void) {
   return 0;
}

int32_t HAL_GetMatchInfo(HAL_MatchInfo* pInfo) {
   strcpy(&(pInfo->eventName[0]), "");
   pInfo->matchNumber = 0;
   return 0;
}



HANDLE hConsoleMutex;

auto initialTxTime = chrono::high_resolution_clock::now();
DWORD initialRxTime = 0;
std::unordered_map<uint32_t, CANMsgAndTimestamp> canMsgMap;
std::queue<CANMsgAndTimestamp> canRxFifo;

static void PrintCANMsg(CANMsgAndTimestamp& canMsg, int isTx)
{
#if 0
   if ((canMsg.msg.ID != 0x01011840) && ((canMsg.msg.ID&0xFFFFE0FF) != 0x0A0B0000))
   {
      WaitForSingleObject(hConsoleMutex, INFINITE);
      std::cout << fmt::format("{:08}  ", canMsg.timestamp.millis - initialRxTime);

      std::cout << (isTx ? "T " : "R ");
      std::cout << fmt::format("{:08X}", canMsg.msg.ID) << format("  {:}  ", (int)canMsg.msg.LEN);
      for (int i = 0; (i < 8) && (i < canMsg.msg.LEN); i++)
      {
         std::cout << fmt::format("{:02X} ", canMsg.msg.DATA[i]);
      }
      std::cout << std::endl;
      ReleaseMutex(hConsoleMutex);
   }
#endif
}

static void RecieveCANMessages()
{
   CANMsgAndTimestamp canMsg;

   while (HalCAN_Receive(&canMsg))
   {
      if (initialRxTime == 0)
      {
         initialRxTime = canMsg.timestamp.millis;
      }

      PrintCANMsg(canMsg, 0);

      // Save CAN message off for the FRC_NetworkCommunication_CANSessionMux_receiveMessage function;
      canMsgMap[canMsg.msg.ID] = canMsg;

      // Save CAN message in the receive FIFO for FRC_NetworkCommunication_CANSessionMux_readStreamSession
      canRxFifo.push(canMsg);
   }
}


int main()
{
   hConsoleMutex = CreateMutex(NULL, FALSE, NULL);

   std::cout << "Playing with Fusion FRC RoboRIO Library\n";
   std::cout << "Point web browser to http://127.0.0.1:5812 to access Playing With Fusion device management page.\n\n";
   bool exit = false;

   HalCAN_Open();

   while (exit == false)
   {
      if (GetAsyncKeyState(VK_ESCAPE))
      {
         exit = true;
      }

      Sleep(1);
      RecieveCANMessages();
   }

   HalCAN_Close();

   std::cout << "Exiting" << std::endl;
}




void FRC_NetworkCommunication_CANSessionMux_openStreamSession(uint32_t* sessionHandle, uint32_t messageID, uint32_t messageIDMask, uint32_t maxMessages, int32_t* status)
{
   // Report success
   *status = 0;
}

void FRC_NetworkCommunication_CANSessionMux_readStreamSession(uint32_t sessionHandle, struct tCANStreamMessage* messages, uint32_t messagesToRead, uint32_t* messagesRead, int32_t* status)
{
   CANMsgAndTimestamp canMsg;

   if ((messagesToRead > 0) && 
       (messages) &&
       (!canRxFifo.empty()))
   {
      canMsg = canRxFifo.front();
      canRxFifo.pop();

      messages->messageID = canMsg.msg.ID;
      messages->dataSize = canMsg.msg.LEN;
      messages->timeStamp = canMsg.timestamp.millis;
      for (int i = 0; i < 8; i++)
      {
         messages->data[i] = canMsg.msg.DATA[i];
      }

      *messagesRead = 1;
      *status = 0;  // Report successcanRxFifo.pop();
   }
   else
   {
      *messagesRead = 0;
      *status = 0;  // Report success
   }
}

void FRC_NetworkCommunication_CANSessionMux_closeStreamSession(uint32_t sessionHandle)
{
   // Do nothing
}

void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t id, uint8_t* data, int y, int z, int32_t* status)
{
   CANMsgAndTimestamp canMsg;

   canMsg.msg.MSGTYPE = PCAN_MESSAGE_EXTENDED;
   canMsg.msg.ID = id;
   canMsg.msg.LEN = y;
   for (int i = 0; (i < 8) && (i<canMsg.msg.LEN); i++)
   {
      canMsg.msg.DATA[i] = data[i];
   }

   auto now = chrono::high_resolution_clock::now();
   auto deltaT = now - initialTxTime;
   auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(deltaT).count();
   canMsg.timestamp.millis = (DWORD)miliseconds + initialRxTime;


   HalCAN_Transmit(&canMsg);

   PrintCANMsg(canMsg, 1);

   // Report success
   *status = 0;
}

void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t* id, uint32_t mask, uint8_t* data, uint8_t* length, uint32_t* timeStamp, int32_t* status)
{
   CANMsgAndTimestamp canMsg;
   if (canMsgMap.find(*id) != canMsgMap.end())
   {
      canMsg = canMsgMap[*id];

      *id         = canMsg.msg.ID;
      *length     = canMsg.msg.LEN;
      *timeStamp  = canMsg.timestamp.millis;

      for (int i = 0; (i < 8) && (i < canMsg.msg.LEN); i++)
      {
         data[i] = canMsg.msg.DATA[i];
      }

      // Report success
      *status = 0;
   }
   else
   {
      // Report error
      *status = 1;
   }
}

int FRC_NetworkCommunication_getWatchdogActive()
{
   // Always return zero (watchdog has not occured)
   return 0;
}
