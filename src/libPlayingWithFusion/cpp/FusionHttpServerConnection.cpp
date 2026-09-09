#include <sstream>
#include <cstdio>
#include <list>
#include <fmt/format.h>
#include <wpinet/UrlParser.h>
#include <wpinet/uv/Tcp.h>
#include "FusionFlashUtil.h"
#include "FusionHttpServerConnection.h"
#include "BattFuelGaugeImpl.h"


namespace uv = wpi::uv;
using namespace std;
using namespace pwf;

extern const size_t resourceVenomLogoLength;
extern const size_t resourceFaviconLength;
extern const unsigned char resourceFavicon[];
extern const unsigned char resourceVenomLogo[];

#include "../../../build/html/bfg.h"
#include "../../../build/html/favicon.h"
#include "../../../build/html/jquery-3.6.0.min.h"
#include "../../../build/html/logo_white.h"
#include "../../../build/html/style.h"

BattFuelGaugeImpl bfg(0, 1);


FusionHttpServerConnection::FusionHttpServerConnection(std::shared_ptr<uv::Stream> stream, FusionFlashUtil &flashUtil) : HttpServerConnection(stream), m_flashUtil(flashUtil) {
   m_request.body.connect_connection([this](std::string_view str, bool isFinal) {ProcessBody(str, isFinal); });
   m_request.messageBegin.connect_connection([this] {ProcessNewRequest(); });
}

void FusionHttpServerConnection::DecodeGetPost(std::string_view str, map<string, string> &dst) {
   size_t startPosn = 0;
   size_t ampPosn;
   size_t eqPosn;

   while (startPosn < str.size())
   {
      ampPosn = str.find_first_of('&', startPosn);
      eqPosn  = str.find_first_of('=', startPosn);

      if (eqPosn != std::string_view::npos)
      {
         string key{str.substr(startPosn, eqPosn-startPosn)};
         string value{str.substr(eqPosn + 1, ampPosn-eqPosn-1)};
         dst[key] = value;
      }

      if (ampPosn == std::string_view::npos)
      {
         break;
      }

      startPosn = ampPosn + 1;
   }
}

void FusionHttpServerConnection::ProcessNewRequest() {
   m_post.clear();
   m_get.clear();
}

void FusionHttpServerConnection::ProcessBody(std::string_view str, bool isFinal) {
   if (isFinal) {
      DecodeGetPost(str, m_post);
   }
}

void FusionHttpServerConnection::BuildCommonHeaders(wpi::raw_ostream& os) {
   os << "Server: PWFDeviceConfigServer/1.0\r\n";
//   os << "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, ";
//   os << "post-check=0, max-age=0\r\n";
//   os << "Pragma: no-cache\r\n";
//   os << "Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n";
}


void FusionHttpServerConnection::ProcessRequest() {
   wpi::UrlParser url{ m_request.GetUrl(),
      m_request.GetMethod() == wpi::HTTP_CONNECT };
   if (!url.IsValid()) {
      // failed to parse URL
      SendError(400);
      return;
   }

   std::string_view path;
   if (url.HasPath()) path = url.GetPath();

   std::string_view query;
   if (url.HasQuery()) {
      query = url.GetQuery();
      DecodeGetPost(query, m_get);
   }

   const bool isGET = m_request.GetMethod() == wpi::HTTP_GET;
   const bool isPOST = m_request.GetMethod() == wpi::HTTP_POST;

   if (isGET || isPOST) {
      if ((path.compare("/") == 0) || (path.compare("/index.html") == 0)) {
         // build HTML root page
         wpi::SmallString<4096> buf;
         wpi::raw_svector_ostream os{ buf };

         //HACK  Redirect to the BFG page
         if (m_post["action"].compare("Live+Data") == 0) {
            SendResponse(302, "OK", "text/html", os.str(), "Location: bfg.html");
            bfg.m_sensorID = stoi(m_post["srcaddr"]);
            bfg.m_deviceId = (pwf_device_t)0;// (pwf_device_t)stoi(m_post["devid"]);
            bfg.m_firmwareVersion = 0;
            bfg.m_serialNumber = 0;// (uint32_t)stoi(m_post["serial"]);
         }
         else
         {
            RenderIndex(os);
            SendResponse(200, "OK", "text/html", os.str(), "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\nPragma: no-cache\r\nExpires: Mon, 3 Jan 2000 12:34:56 GMT\r\n");
         }
      }
      else if (path.compare("/bfg_state.json") == 0) {
         wpi::SmallString<4096> buf;
         wpi::raw_svector_ostream os{ buf };

         RenderBfgStateJSONWithHash(os);
         SendResponse(200, "OK", "text/json", os.str(), "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\nPragma: no-cache\r\nExpires: Mon, 3 Jan 2000 12:34:56 GMT\r\n");
      }
      else if (path.compare("/bfg.cgi") == 0) {
         wpi::SmallString<4096> buf;
         wpi::raw_svector_ostream os{ buf };

         RenderBfgCgi(os);
         SendResponse(200, "OK", "text/json", os.str(), "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\nPragma: no-cache\r\nExpires: Mon, 3 Jan 2000 12:34:56 GMT\r\n");
      }
      else if (path.compare("/favicon.png") == 0) {
         std::string_view ref((char*)html_favicon_png, html_favicon_png_len);
         SendStaticResponse(200, "OK", "image/png", ref, false, "Cache-Control: max-age=3600\r\n");
      }
      else if (path.compare("/logo_white.svg") == 0) {
         std::string_view ref((char*)html_logo_white_svg, html_logo_white_svg_len);
         SendStaticResponse(200, "OK", "image/svg+xml", ref, false, "Cache-Control: max-age=3600\r\n");
      }
      else if (path.compare("/bfg.html") == 0) {
#ifdef USE_PWFUSION_TEST_Server
         std::string ref((char*)html_bfg_html, html_bfg_html_len);

         std::string oldText = "playingwithfusion.com";
         std::string newText = "pwfusion.test";
         size_t idx = 0;
         while ((idx = ref.find(oldText, idx)) != std::string::npos) {
            ref.replace(idx, oldText.length(), newText);
            // Move the search position forward by the length of the replacement string
            idx += newText.length();
         }
#else
         std::string_view ref((char*)html_bfg_html, html_bfg_html_len);
#endif
         SendStaticResponse(200, "OK", "text/html", ref, false, "Cache-Control: max-age=3600\r\n");
      }
      else if (path.compare("/jquery-3.6.0.min.js") == 0) {
         std::string_view ref((char*)html_jquery_3_6_0_min_js, html_jquery_3_6_0_min_js_len);
         SendStaticResponse(200, "OK", "text/javascript", ref, false, "Cache-Control: max-age=3600\r\n");
      }
      else if (path.compare("/style.css") == 0) {
         std::string_view ref((char*)html_style_css, html_style_css_len);
         SendStaticResponse(200, "OK", "text/css", ref, false, "Cache-Control: max-age=3600\r\n");
      }
      else {
         SendError(404, "Resource not found");
      }
   }
   else {
      SendError(404, "Resource not found");
   }
}

void FusionHttpServerConnection::RenderBfgStateJSONWithHash(wpi::raw_svector_ostream& outStream) {
   bfg.ExecuteTask();
   outStream << bfg.GetBfgStateJSON();
}

void FusionHttpServerConnection::RenderBfgCgi(wpi::raw_svector_ostream& outStream) {
   try {
      if (m_post["action"].compare("setname") == 0) {
         bfg.SetNickname(m_post["name"]);
      }
      else if (m_post["action"].compare("setmfg") == 0) {
         bfg.SetManufacturer((BattFuelGauge_BattMfg)std::stoi(m_post["mfg"]));
      }
      else if (m_post["action"].compare("setcalibration") == 0) {
         bfg.SetCalibrationVals((uint32_t)(unsigned long long)std::stoll(m_post["currentoffset"]), (uint32_t)(unsigned long long)std::stoll(m_post["currentgain"]));
      }
      else if (m_post["action"].compare("setage") == 0) {
         bfg.SetBatteryAge(std::stof(m_post["capacity"]), (uint32_t)(std::stof(m_post["age"]) * (24.0 * 3600.0)), (uint32_t)(unsigned long long)std::stol(m_post["numcycles"]));
      }
      else if ((m_post["action"].compare("resetbfg") == 0) && (m_post["password"].compare("BOMBSAWAY") == 0)) {
         bfg.ResetAllBatteryStats();
      }
      else if (m_post["action"].compare("setdisp") == 0) {
         bfg.SetDisplay(std::stoi(m_post["inverted"]));
      }
      else if (m_post["action"].compare("caloffset") == 0) {
         bfg.InitiateCurrentOffsetCalibration();
      }
      else if (m_post["action"].compare("calgain") == 0) {
         bfg.InitiateCurrentGainCalibration();
      }

      outStream << "{\"success\":true}\n";
   }
   catch (const std::exception& e)
   {
      outStream << "{\"success\":false, \"errortext\":\"" << e.what() << "\"}\n";
   }
}

void FusionHttpServerConnection::RenderIndex(wpi::raw_svector_ostream &outStream) {
   //Handle Form submissions first
   if (m_post["action"].compare("Identify") == 0) {
      //Identify Controller by flashing it's LEDs
      m_flashUtil.IdentifyController((pwf_device_t)stoi(m_post["devid"]), (uint32_t)stoi(m_post["serial"]));
   }
   else if (m_post["action"].compare("Change+ID") == 0) {
      // Change Device ID
      if (m_post["devid"].length() && m_post["serial"].length()) {
         m_flashUtil.SetControllerId((pwf_device_t)stoi(m_post["devid"]), (uint32_t)stoi(m_post["serial"]), (uint8_t)stoi(m_post["newid"]));
      }
   }
   else if (m_post["action"].compare("Update") == 0) {
      //Flash Latest image
      if (m_post["devid"].length() && m_post["serial"].length()) {
 //        m_flashUtil.BeginFlash((pwf_device_t)stoi(m_post["devid"]), (uint32_t)stoi(m_post["serial"]));
      }
   }

   //Render HTML
   outStream << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>PWF Devices</title>";
   outStream << "<link rel=\"stylesheet\" href=\"style.css\">";
   outStream << "<link rel=\"shortcut icon\" href=\"favicon.ico\">";
   outStream << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
 
   FlashingState flashState = m_flashUtil.GetFlashState();
#ifndef WIN32
   if (flashState == FlashingState::InProcess) 
#endif // WIN32
   {
      outStream << "<meta http-equiv=\"refresh\" content=\"2\">";
   }

   outStream << "</head><body>";

   outStream << "<div class=\"document\">";
   outStream << "   <div class=\"header\">";
   outStream << "      <image src=\"logo_white.svg\" width=\"225\" height=\"77\" style=\"float:left;\">";
   outStream << "         <h1>Device Programming</h1>";
   outStream << "         <p>All CAN sensors/actuators</p>";
   outStream << "   </div>";
   outStream << "   <div class=\"content\">";
   outStream << "      <p>All PlayingWithFusion CAN devices (motor controllers, time-of-flight sensors, battery monitors, etc) may be";
   outStream << "         managed";
   outStream << "         through this page. Each device must be assigned an identifier which is used when connecting to the device";
   outStream << "         through the Robot application. Devices of different types (ie. a motor controller or a TOF sensor) may";
   outStream << "         share the same ID, but each device of the same type must have a unique ID.</p>";
   outStream << "      <p>Use the 'Identify' button to flash the LEDs of a particular device for several seconds. This is helpful";
   outStream << "         to be sure the correct ID is assigned to the right device. Identifiers are stored in non-volatile memory";
   outStream << "         and are preserved, even after the battery is disconnected.</p>";
   outStream << "      <p>Firmware may also be updated for each device through this page by pressing the 'Update' button. If the";
   outStream << "         'Update' button is hidden the firmware is already up to date.</p>";

   if (flashState == FlashingState::InProcess) {
      outStream << "<div class=\"error\">Reprogramming Controller, " << std::to_string(m_flashUtil.GetFlashPercentComplete()) << "% complete</div>";
   }
   else if (flashState == FlashingState::Success) {
      outStream << "<div class=\"error\">Sucessfully reprogrammed controller</div>";
   }
   else if (flashState == FlashingState::Error) {
      outStream << "<div class=\"error\">An error occured during reprogramming.  Press the 'Update' button again to retry</div>";
   }


   outStream << "<div class=\"param-group\">";
   outStream << "         <h2>Detected Devices</h2>";
   outStream << "         <table>";
   outStream << "            <tr>";
   outStream << "               <th>Device Name</th>";
   outStream << "               <th>Serial<br />Number</th>";
   outStream << "               <th>ID</th>";
   outStream << "               <th></th>";
   outStream << "               <th></th>";
   outStream << "               <th></th>";
   outStream << "               <th>Firmware<br />Version</th>";
   outStream << "               <th>Latest<br />Version</th>";
   outStream << "               <th>&nbsp;</th>";
   outStream << "            </tr>";

   list<PWFController> controllerList = m_flashUtil.GetControllerList();
   for (auto controller = controllerList.begin(); controller != controllerList.end(); controller++) {
      outStream << "<tr><form method = \"post\"><td class=\"device-name\">";
      switch (controller->device) {
      case DEV_TOF_SENSOR:
      case DEV_TOF_SENSOR_CK128:
      case DEV_ANALOG_TOF_SENSOR_CK128:
      case DEV_TOF_SENSOR_CK256:
         outStream << "Time Of Flight Sensor";
         break;

      case DEV_VENOM_MOTOR_CONTROLLER_PROTO:
      case DEV_VENOM_MOTOR_CONTROLLER:
         outStream << "Venom Motor Controller";
         break;

      case DEV_BFG_SENSOR:
         outStream << "Battery Fuel Gauge";
         break;

      default:
         outStream << "Unknown Device";
         break;
      }
      outStream << "</td><td>" << std::to_string(controller->serialNumber) << "</td>";
      outStream << "<td class=\"button\"><input type=\"text\" name=\"newid\" style=\"width:2em;\" value=\"" << std::to_string((long)controller->id) << "\"></td>";
      outStream << "<td class=\"button\"><input type=\"submit\" name=\"action\" value=\"Change ID\"></td>";
      outStream << "<td class=\"button\"><input type=\"submit\" name=\"action\" value=\"Identify\"></td>";

      if (controller->device == DEV_BFG_SENSOR)
      {
         outStream << "<td><input type=\"submit\" name=\"action\" value=\"Live Data\" class=\"live-data\"></td>";
      }
      else
      {
         outStream << "<td></td>";
      }

      char verString[17];
      char latestVerString[17];
      std::snprintf(verString, sizeof(verString), "%i.%02i", controller->firmwareVersion / 100, controller->firmwareVersion % 100);
      std::snprintf(latestVerString, sizeof(latestVerString), "%i.%02i", controller->latestFwVersion / 100, controller->latestFwVersion % 100);

      if (controller->firmwareVersion >= controller->latestFwVersion) {
         outStream << "<td class=\"fw-old\"><font color=\"#257226\">" << verString << "</font></td>";
         outStream << "<td class=\"fw-old\"><font color=\"#257226\">" << latestVerString << "</font></td>";
         outStream << "<td>&nbsp;</td>";
      }
      else {
         outStream << "<td class=\"fw-old\"><font color=\"#C63527\">" << verString << "</font></td>";
         outStream << "<td class=\"fw-old\"><font color=\"#C63527\">" << latestVerString << "</font></td>";
         if (flashState != FlashingState::InProcess) {
            outStream << "<td class=\"button\"><input type=\"submit\" name=\"action\" value=\"Update\"></td>";
         }
      }

      outStream << "<input type=\"hidden\" name=\"srcaddr\" value=\"" << std::to_string((long)controller->id) << "\">";
      outStream << "<input type=\"hidden\" name=\"devid\" value=\"" << std::to_string((long)controller->device) << "\">";
      outStream << "<input type=\"hidden\" name=\"serial\" value=\"" << std::to_string((long)controller->serialNumber) << "\">";

      outStream << "</form></tr>";
   }

   outStream << "</table>";
   outStream << "</div></div></div></body></html>";
}

