#pragma once

#include <map>
#include <string>
#include <wpinet/HttpServerConnection.h>
#include <wpinet/raw_uv_ostream.h>
#include "FusionFlashUtil.h"

namespace uv = wpi::uv;
using namespace std;

namespace pwf {

class FusionHttpServerConnection : public wpi::HttpServerConnection {
public:
   explicit FusionHttpServerConnection(std::shared_ptr<uv::Stream> stream, FusionFlashUtil &flashUtil);

protected:
   void ProcessNewRequest();
   void ProcessRequest() override;
   void BuildCommonHeaders(wpi::raw_ostream& os) override;

   void ProcessBody(std::string_view str, bool isFinal);
   void RenderIndex(wpi::raw_svector_ostream &outStream);
   void RenderBfgStateJSONWithHash(wpi::raw_svector_ostream& outStream);
   void RenderBfgCgi(wpi::raw_svector_ostream& outStream);
   void DecodeGetPost(std::string_view str, map<string, string> &dst);

private:
   map<string, string> m_post;
   map<string, string> m_get;
   FusionFlashUtil &m_flashUtil;
};

}