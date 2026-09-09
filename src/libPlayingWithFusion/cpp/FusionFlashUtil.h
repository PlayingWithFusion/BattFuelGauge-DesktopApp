#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <list>
#include "FirmwareImage.h"
#include "PWFController.h"

namespace pwf {
   enum FlashingState
   {
      Idle,
      InProcess,
      Success,
      Error
   };

   enum ReprogSubCommand
   {
      Unknown = 10,
      ReqToProgram = 2,
      Erase = 3,
      ReqToSendBlock = 4,
      SendData = 5,
      Done = 6,
      NAK = 0,
      Resend = 7
   };

   enum AckResult
   {
      Ack,
      Nack,
      Timeout
   };


   class FusionFlashUtil {
   public:
      explicit FusionFlashUtil();
      ~FusionFlashUtil();

      void BeginFlash(pwf_device_t device, uint32_t serialNumber);
      FlashingState GetFlashState();
      unsigned int GetFlashPercentComplete();
      std::string GetLastFlashError();

      std::list<PWFController> GetControllerList();

      PWFController * GetControllerById(pwf_device_t device, uint8_t id);
      void SetControllerId(pwf_device_t device, uint32_t serialNumber, uint8_t id);
      void IdentifyController(pwf_device_t device, uint32_t serialNumber);
      void UnlockController(pwf_device_t device, uint32_t serialNumber);

   private:
      const unsigned int m_kMaxRetries = 4;
      const unsigned int m_kAckTimeout = 2000;

      std::atomic<unsigned int> m_percentComplete = {0};
      std::atomic<FlashingState> m_state = {FlashingState::Idle};
//      std::atomic<std::string> m_errorString;

      std::atomic<bool> m_terminateThreads = { false };
      std::thread *m_pFlashThread = 0;
      std::thread *m_pIdentifyThread = 0;
      std::list<PWFController> m_controllerList;
      unsigned int m_segmentsPerAck = 16;    // Default value
      unsigned int m_blockSize = 4096;       // Default block size in old bootloader versions
      unsigned int m_skipFFBlocks = false;
      unsigned int m_skip00Blocks = false;
      uint32_t m_serialNumber;
      uint32_t m_deviceID;

      static void CallIdentifyHeartbeatTask(void *pFlashUtil);
      void IdentifyHeartbeatTask();

      static void CallFlashTask(void *pFlashUtil);
      void FlashTask();
   };

}