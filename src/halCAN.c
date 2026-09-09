#include <stdio.h>

#include "halCAN.h"
#include "candle/candle.h"


TPCANHandle hPCAN = PCAN_USBBUS1;
candle_handle hCandle;


typedef enum
{
   DRIVER_NONE,
   DRIVER_PCAN,
   DRIVER_CANDLE
} can_driver_t;

can_driver_t driver = DRIVER_NONE;

void HalCAN_Open()
{
   candle_list_handle list;
   uint8_t num_devs;

   if (candle_list_scan(&list))
   {
      candle_list_length(list, &num_devs);
      if (num_devs > 0)
      {
         candle_dev_get(list, 0, &hCandle);
         if (candle_dev_open(hCandle))
         {
            printf("Found Candle compatible CAN device\n");

            driver = DRIVER_CANDLE;
            candle_channel_set_bitrate(hCandle, 0, 1000000);
            candle_channel_start(hCandle, 0, CANDLE_MODE_HW_TIMESTAMP);
         }
      }
      candle_list_free(list);
   }

   if (driver == DRIVER_NONE)
   {

      if (CAN_Initialize(hPCAN, PCAN_BAUD_1M, 0, 0, 0) == PCAN_ERROR_OK)
      {
         driver = DRIVER_PCAN;
         printf("Found PEAK PCAN compatible CAN device\n");
      }
      else
      {
         driver = DRIVER_NONE;
         printf("No compatible CAN interface detected\n");
      }

   }
}


void HalCAN_Close()
{
   switch (driver)
   {
      case DRIVER_PCAN:
         CAN_Uninitialize(hPCAN);
         break;

      case DRIVER_CANDLE:
         candle_channel_stop(hCandle, 0);
         candle_dev_close(hCandle);
         break;

      default:
         break;
   }
}


BOOL HalCAN_Receive(CANMsgAndTimestamp* pMsg)
{
   uint8_t success = FALSE;
   candle_frame_t candleFrame;

   switch (driver)
   {
   case DRIVER_PCAN:
      success = (CAN_Read(hPCAN, &(pMsg->msg), &(pMsg->timestamp)) == PCAN_ERROR_OK);
      break;

   case DRIVER_CANDLE:
      success = candle_frame_read(hCandle, &candleFrame, 0);

      pMsg->msg.ID      = candle_frame_id(&candleFrame);
      pMsg->msg.MSGTYPE = candle_frame_is_extended_id(&candleFrame) ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD;
      pMsg->msg.LEN     = candle_frame_dlc(&candleFrame);
      pMsg->timestamp.millis  = candle_frame_timestamp_us(&candleFrame) / 1000;
      memcpy(pMsg->msg.DATA, candleFrame.data, 8);
      break;

   default:
      break;
   }

   return success;
}

void HalCAN_Transmit(CANMsgAndTimestamp* pMsg)
{
   candle_frame_t candleFrame;
   bool success;

   switch (driver)
   {
   case DRIVER_PCAN:
      CAN_Write(hPCAN, &(pMsg->msg));
      break;

   case DRIVER_CANDLE:
      candleFrame.can_id = pMsg->msg.ID;
      candleFrame.can_dlc = pMsg->msg.LEN;
      candleFrame.channel = 0;
      candleFrame.flags = 0;

      if (pMsg->msg.MSGTYPE == PCAN_MESSAGE_EXTENDED)
      {
         candleFrame.can_id |= 0x80000000;
      }

      memcpy(candleFrame.data, pMsg->msg.DATA, 8);
      success = candle_frame_send(hCandle, 0, &candleFrame, 1, 1000);
      break;

   default:
      break;
   }
}
