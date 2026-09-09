#include <wpinet/uv/Tcp.h>
#include "FusionHttpServer.h"

namespace uv = wpi::uv;
using namespace pwf;


FusionHttpServer::FusionHttpServer(FusionFlashUtil &flashUtil) {
   FusionFlashUtil &flashUtilLocal = flashUtil;
   m_eventLoop.ExecAsync([this, &flashUtilLocal](uv::Loop& loop) {EventLoopTask(loop, flashUtilLocal); });
}


FusionHttpServer::~FusionHttpServer() {
   m_eventLoop.Stop();
}


void FusionHttpServer::EventLoopTask(uv::Loop& loop, FusionFlashUtil &flashUtil) {
   auto tcp = uv::Tcp::Create(loop);

   // bind to listen address and port
   tcp->Bind("", 5812);

   // when we get a connection, accept it and start reading
   FusionFlashUtil &flashUtilLocal = flashUtil;
   tcp->connection.connect([srv = tcp.get(), &flashUtilLocal]{
      auto tcp = srv->Accept();
      if (!tcp) return;

      auto conn = std::make_shared<FusionHttpServerConnection>(tcp, flashUtilLocal);
      tcp->SetData(conn);
   });

   // start listening for incoming connections
   tcp->Listen();
}
