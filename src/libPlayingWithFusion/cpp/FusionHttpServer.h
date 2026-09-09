#pragma once
#include <wpinet/EventLoopRunner.h>
#include <wpinet/uv/Loop.h>
#include "FusionHttpServerConnection.h"
#include "FusionFlashUtil.h"


namespace pwf {

   class FusionHttpServer {
   public:
      explicit FusionHttpServer(FusionFlashUtil &flashUtil);
      ~FusionHttpServer();

   private:
      wpi::EventLoopRunner m_eventLoop;
      //FusionFlashUtil &m_flashUtil;

      void EventLoopTask(uv::Loop& loop, FusionFlashUtil &flashUtil);
   };

}