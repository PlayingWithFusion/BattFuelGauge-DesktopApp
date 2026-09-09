#pragma once
#include<stdint.h>


struct tCANStreamMessage {
   /** The message ID */
   uint32_t messageID;
   /** The packet received timestamp (based off of CLOCK_MONOTONIC) */
   uint32_t timeStamp;
   /** The message data */
   uint8_t data[8];
   /** The size of the data received (0-8 bytes) */
   uint8_t dataSize;
};


extern void FRC_NetworkCommunication_CANSessionMux_openStreamSession(uint32_t* sessionHandle, uint32_t messageID, uint32_t messageIDMask, uint32_t maxMessages, int32_t* status);
extern void FRC_NetworkCommunication_CANSessionMux_readStreamSession(uint32_t sessionHandle, struct tCANStreamMessage* messages, uint32_t messagesToRead, uint32_t* messagesRead, int32_t* status);
extern void FRC_NetworkCommunication_CANSessionMux_closeStreamSession(uint32_t sessionHandle);

extern void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t id, uint8_t* data, int dataSize, int periodMs, int32_t* status);
extern void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t* id, uint32_t mask, uint8_t* data, uint8_t* length, uint32_t* timeStamp, int32_t* status);

extern int FRC_NetworkCommunication_getWatchdogActive();
