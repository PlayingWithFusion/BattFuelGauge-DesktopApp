#ifndef HAL_CAN_H
#define HAL_CAN_H

#include <windows.h>
#include "PCAN/pcanbasic.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
   TPCANMsg msg;
   TPCANTimestamp timestamp;
} CANMsgAndTimestamp;


extern void HalCAN_Open(void);
extern void HalCAN_Close(void);
extern BOOL HalCAN_Receive(CANMsgAndTimestamp* pMsg);
extern void HalCAN_Transmit(CANMsgAndTimestamp* pMsg);


#ifdef __cplusplus
}
#endif

#endif // HAL_CAN_H
