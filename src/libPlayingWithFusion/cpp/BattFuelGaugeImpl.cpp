#include "FusionFlashUtil.h"
#include "BattFuelGaugeImpl.h"
#include "math.h"
#include <chrono>
#include <fstream>
#include <fmt/format.h>
#include <wpi/fs.h>
#include <hal/DriverStation.h>
#include <hal/HALBase.h>
#include "XxHash32.h"
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
#include <FRC_NetworkCommunication/FRCComm.h>
#else
static void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t id, uint8_t *data, int y, int z, int32_t *status) {

}


static void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t *id, uint32_t mask, uint8_t *data, uint8_t *length, uint32_t *timeStamp, int32_t *status) {
   *length = 0;
}
#endif

pwf::FusionFlashUtil flashUtil;

using namespace pwf;

#ifndef BFG_HASH_SECRET
#define BFG_HASH_SECRET          (0x12345678U)
#endif // BFG_HASH_SECRET


//HAL_CANManufacturer::HAL_CAN_Man_kTeamUse
BattFuelGaugeImpl::BattFuelGaugeImpl(uint8_t sensorID, uint8_t ignoreSensorID) {
   m_sensorID    = sensorID;
   m_sensorIdMask = ignoreSensorID ? 0x1FFFFF00 : 0x1FFFFFFF;

  //  periodicHelper.AddTask(this);
}

BattFuelGaugeImpl::~BattFuelGaugeImpl() {
  //  periodicHelper.RemoveTask(this);
}


void BattFuelGaugeImpl::GetPWFControllerInfo() {
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
 //  if ((m_firmwareVersion==0) && (m_serialNumber==0))
   {
      PWFController *pController = flashUtil.GetControllerById(pwf_device_t::DEV_BFG_SENSOR, m_sensorID);
      if (pController != NULL)
      {
         m_deviceId        = pController->device;
         m_firmwareVersion = pController->firmwareVersion;
         m_serialNumber    = pController->serialNumber;
      }
   }
#endif
}


void BattFuelGaugeImpl::IdentifySensor() {
   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.IdentifyController(m_deviceId, m_serialNumber);
#endif
}

uint32_t BattFuelGaugeImpl::GetFirmwareVersion() {
   GetPWFControllerInfo();
   return m_firmwareVersion;
}

uint32_t BattFuelGaugeImpl::GetSerialNumber() {
   GetPWFControllerInfo();
   return m_serialNumber;
}

std::string BattFuelGaugeImpl::GetBfgStateJSON() {
   std::string bfgJson;
   bfgJson.reserve(1200);
   bfgJson += "{\n";
   bfgJson += "\"nickname\":\"" + GetNickname() + "\",\n";
   bfgJson += fmt::format("\"bfg_serial\":{:},\n", GetSerialNumber());
   bfgJson += fmt::format("\"bfg_firmware\":{:},\n", GetFirmwareVersion());
   bfgJson += fmt::format("\"batt_capacity\":{:},\n",        m_capacity_Ah);
   bfgJson += fmt::format("\"batt_rated_capacity\":{:},\n",  m_ratedCapacity_Ah);
   bfgJson += "\"batt_mfg\":\"" + GetManufacturerString() + "\",\n";
   bfgJson += fmt::format("\"batt_mfg_id\":{:},\n",          (int)m_battManufacturer);
   bfgJson += fmt::format("\"bat_age\":{:},\n",              m_battAge_s);
   bfgJson += fmt::format("\"bat_num_cycles\":{:},\n",       m_battNumCycles);
   bfgJson += "\"bfg_state\":\"" + GetChargeStateString() + "\",\n";

   bfgJson += fmt::format("\"cal_current_offset\":{:},\n",   m_measCurrentOffset);
   bfgJson += fmt::format("\"cal_current_gain\":{:},\n",     m_measCurrentGain);

   bfgJson += fmt::format("\"voltage\":{:},\n",              m_voltage_V);
   bfgJson += fmt::format("\"current\":{:},\n",              m_current_A);
   bfgJson += fmt::format("\"dod_ah\":{:},\n",               m_dod_Ah);
   bfgJson += fmt::format("\"dod_wh\":{:},\n",               m_dod_Wh);
   bfgJson += fmt::format("\"soc_ah\":{:},\n",               m_soc_Ah);
   bfgJson += fmt::format("\"effective_cap_ah\":{:},\n",     m_effectiveCapacity_Ah);

   bfgJson += fmt::format("\"cycle_min_v\":{:},\n",          m_cycleStats.minVoltage_V);
   bfgJson += fmt::format("\"cycle_max_v\":{:},\n",          m_cycleStats.maxVoltage_V);
   bfgJson += fmt::format("\"cycle_min_i\":{:},\n",          m_cycleStats.minCurrent_A);
   bfgJson += fmt::format("\"cycle_max_i\":{:},\n",          m_cycleStats.maxCurrent_A);
   bfgJson += fmt::format("\"cycle_starting_v\":{:},\n",     m_cycleStats.startingVoltage_V);
   bfgJson += fmt::format("\"cycle_rms_i\":{:},\n",          m_cycleStats.rmsDischargeCurrent_A);
   bfgJson += fmt::format("\"cycle_min_dod_ah\":{:},\n",     m_cycleStats.minDOD_Ah);
   bfgJson += fmt::format("\"cycle_min_dod_wh\":{:},\n",     m_cycleStats.minDOD_Wh);
   bfgJson += fmt::format("\"cycle_max_dod_ah\":{:},\n",     m_cycleStats.maxDOD_Ah);
   bfgJson += fmt::format("\"cycle_max_dod_wh\":{:},\n",     m_cycleStats.maxDOD_Wh);
   bfgJson += fmt::format("\"cycle_charge_time\":{:},\n",    m_cycleStats.chargeTime_s);
   bfgJson += fmt::format("\"cycle_discharge_time\":{:},\n", m_cycleStats.dischargeTime_s);

   bfgJson += fmt::format("\"match_min_v\":{:},\n",          m_matchStats.minVoltage_V);
   bfgJson += fmt::format("\"match_max_v\":{:},\n",          m_matchStats.maxVoltage_V);
   bfgJson += fmt::format("\"match_min_i\":{:},\n",          m_matchStats.minCurrent_A);
   bfgJson += fmt::format("\"match_max_i\":{:},\n",          m_matchStats.maxCurrent_A);
   bfgJson += fmt::format("\"match_starting_v\":{:},\n",     m_matchStats.startingVoltage_V);
   bfgJson += fmt::format("\"match_rms_i\":{:},\n",          m_matchStats.rmsDischargeCurrent_A);
   bfgJson += fmt::format("\"match_min_dod_ah\":{:},\n",     m_matchStats.minDOD_Ah);
   bfgJson += fmt::format("\"match_max_dod_ah\":{:},\n",     m_matchStats.maxDOD_Ah);
   bfgJson += fmt::format("\"match_charge_time\":{:},\n",    m_matchStats.chargeTime_s);
   bfgJson += fmt::format("\"match_discharge_time\":{:},\n", m_matchStats.dischargeTime_s);
   bfgJson += fmt::format("\"match_active_time\":{:},\n",    m_matchStats.activeTime_s);
   bfgJson += fmt::format("\"match_start_dod_ah\":{:},\n",   m_matchStats.startDOD_Ah);
   bfgJson += fmt::format("\"match_start_dod_wh\":{:},\n",   m_matchStats.startDOD_Wh);
   bfgJson += fmt::format("\"match_end_dod_ah\":{:},\n",     m_matchStats.endDOD_Ah);
   bfgJson += fmt::format("\"match_end_dod_wh\":{:}\n",      m_matchStats.endDOD_Wh);
   bfgJson += "}";


   HAL_MatchInfo matchInfo;
   HAL_GetMatchInfo(&matchInfo);

   std::string fullJson;
   bfgJson.reserve(1200);
   fullJson = "{";
   fullJson += "\"event_name\":\"" + std::string(matchInfo.eventName) + "\",\n";
   fullJson += fmt::format("\"match_number\":{:},\n", matchInfo.matchNumber);
   fullJson += fmt::format("\"team_number\":{:},\n",  HAL_GetTeamNumber());

   uint32_t h = XXHash32::Hash(bfgJson.c_str(), (uint32_t)bfgJson.length(), BFG_HASH_SECRET);
   fullJson += "\"bfg_data\":" + bfgJson + ",\"hash\":\"" + XXHash32::ToHex8(h) + "\"}";

   return fullJson;
}


static std::string GetLogDirectory() {
   std::error_code ec;
   std::string logDir;

#ifdef __FRC_ROBORIO__
   // prefer a mounted USB drive if one is accessible
   auto s = fs::status("/u", ec);
   if (!ec && fs::is_directory(s) &&
      (s.permissions() & fs::perms::others_write) != fs::perms::none) 
   {
      logDir = "/u/logs";
   }
   else
   {
      logDir = "/home/lvuser/logs";
   }
#else
   logDir = fs::current_path().string() + "/logs";
#endif
   fs::create_directory(logDir, ec);

   return logDir;
}

static std::string GetLogFilename() {
   HAL_MatchInfo matchInfo;
   HAL_GetMatchInfo(&matchInfo);

   char matchTypeChar;
   switch (matchInfo.matchType)
   {
      case HAL_kMatchType_practice:
         matchTypeChar = 'P';
         break;
      case HAL_kMatchType_qualification:
         matchTypeChar = 'Q';
         break;
      case HAL_kMatchType_elimination:
         matchTypeChar = 'E';
         break;
      default:
         matchTypeChar = '_';
         break;
   }

   std::time_t now = std::time(nullptr);
   std::tm* t = std::gmtime(&now);
   std::string filename = fmt::format( "BFGSnapshot_{:04}{:02}{:02}_{:02}{:02}{:02}_{}_{}{}.json",
                                       t->tm_year + 1900, t->tm_mon  + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,
                                       matchInfo.eventName, matchTypeChar, matchInfo.matchNumber );
   return filename;
}

void BattFuelGaugeImpl::SaveLog(std::string filename) {
   if (filename.empty())
   {
      filename = GetLogDirectory() + "/" + GetLogFilename();
   }

   std::ofstream logFile(filename);
   if (logFile.is_open())
   {
      logFile << GetBfgStateJSON();
      logFile.close();
   }
}

bool BattFuelGaugeImpl::IsConnected() const {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    return (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPowerMsgRx).count() < 2000);
}

uint8_t BattFuelGaugeImpl::GetSensorID() const {
   return m_sensorID;
}

std::string BattFuelGaugeImpl::GetChargeStateString() const {
   std::string result = "---";

   switch (m_chargeState)
   {
      case BattFuelGauge_ChargeState_kDischarge:
         result = "Discharging";
         break;

      case BattFuelGauge_ChargeState_kConstICharge:
      case BattFuelGauge_ChargeState_kConstVCharge:
      case BattFuelGauge_ChargeState_kTrickleCharge:
         result = "Charging";
         break;

      case BattFuelGauge_ChargeState_kIdle:
         result = "Idle";
         break;

      case BattFuelGauge_ChargeState_kMeasureOCV:
         result = "Open Circuit";
         break;

      case BattFuelGauge_ChargeState_kInit:
      default:
         result = "Discharging";
         break;
   }

   return result;
}


std::string BattFuelGaugeImpl::GetManufacturerString() const
{
   std::string result;

   switch (m_battManufacturer)
   {
      case BattFuelGauge_BattMfg_kDuracell:
         result = "Duracell";
         break;

      case BattFuelGauge_BattMfg_kEnergizer:
         result = "Energizer";
         break;

      case BattFuelGauge_BattMfg_kInterstate:
         result = "Interstate";
         break;

      case BattFuelGauge_BattMfg_kMightyMax:
         result = "MightyMax";
         break;

      case BattFuelGauge_BattMfg_kMKPowered:
         result = "MK Powered";
         break;

      case BattFuelGauge_BattMfg_kPowerSonic:
         result = "Power-Sonic";
         break;

      default:
         result = "Unknown";
         break;
   }

   return result;
}


std::string BattFuelGaugeImpl::GetNickname() {
   m_szNickname[sizeof(m_szNickname)-1] = 0;
   return m_szNickname;
}

void BattFuelGaugeImpl::SetNickname(std::string name)
{
   int32_t status;
   
   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   std::size_t len = name.copy(m_szNickname, sizeof(m_szNickname)-1);
   m_szNickname[len] = 0;

   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetNickname1 | m_sensorID, (uint8_t*)&m_szNickname[0], 8, 0, &status);
   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetNickname2 | m_sensorID, (uint8_t*)&m_szNickname[8], 3, 0, &status);
}


void BattFuelGaugeImpl::SetManufacturer(BattFuelGauge_BattMfg mfgId)
{
   int32_t status;
   uint8_t data[8];

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = 0xFF;
   data[1] = 0xFF;
   data[2] = 0xFF;
   data[3] = (uint8_t)mfgId;
   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetNickname2 | m_sensorID, &data[0], 4, 0, &status);
}




void BattFuelGaugeImpl::SetCalibrationVals(uint32_t currentOffset, uint32_t currentGain)
{
   int32_t status;
   uint8_t data[8];

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = (currentOffset      ) & 0xFF;
   data[1] = (currentOffset >>  8) & 0xFF;
   data[2] = (currentOffset >> 16) & 0xFF;
   data[3] = (currentOffset >> 24) & 0xFF;

   data[4] = (currentGain        ) & 0xFF;
   data[5] = (currentGain   >>  8) & 0xFF;
   data[6] = (currentGain   >> 16) & 0xFF;
   data[7] = (currentGain   >> 24) & 0xFF;

   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetCalibration | m_sensorID, &data[0], 8, 0, &status);
}


void BattFuelGaugeImpl::SetBatteryAge(double capacity_Ah, uint32_t age_s, uint16_t numCycles)
{
   int32_t status;
   uint8_t data[8];
   uint16_t capacity_mAh;

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = (age_s             ) & 0xFF;
   data[1] = (age_s        >>  8) & 0xFF;
   data[2] = (age_s        >> 16) & 0xFF;
   data[3] = (age_s        >> 24) & 0xFF;

   capacity_mAh = (uint16_t)(capacity_Ah * 1000.0);
   data[4] = (capacity_mAh      ) & 0xFF;
   data[5] = (capacity_mAh >>  8) & 0xFF;

   data[6] = (numCycles         ) & 0xFF;
   data[7] = (numCycles    >>  8) & 0xFF;
   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetHealth | m_sensorID, &data[0], 8, 0, &status);
}


void BattFuelGaugeImpl::ResetAllBatteryStats()
{
   int32_t status;
   uint8_t data[8];

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = 0xFF;
   data[1] = 0xFF;
   data[2] = 0xFF;
   data[3] = 0xFF;
   data[4] = 0x69;  // Magic number to reset the BFG
   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetNickname2 | m_sensorID, &data[0], 5, 0, &status);
}


void BattFuelGaugeImpl::InitiateCurrentOffsetCalibration()
{
   int32_t status;
   uint8_t data[8];

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = 0x0B;   // Factory Cal
   data[1] = (m_serialNumber & 0xFF);
   data[2] = (m_serialNumber >> 8);
   data[3] = (m_serialNumber >> 16);
   data[4] = (m_deviceId & 0xFF);
   data[5] = (m_deviceId >> 8);
   data[6] = 1;      // Offset calibration

   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::Reprog, &data[0], 7, 0, &status);
}


void BattFuelGaugeImpl::InitiateCurrentGainCalibration()
{
   int32_t status;
   uint8_t data[8];

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = 0x0B;   // Factory Cal
   data[1] = (m_serialNumber & 0xFF);
   data[2] = (m_serialNumber >> 8);
   data[3] = (m_serialNumber >> 16);
   data[4] = (m_deviceId & 0xFF);
   data[5] = (m_deviceId >> 8);
   data[6] = 2;      // Gain calibration

   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::Reprog, &data[0], 7, 0, &status);
}


void BattFuelGaugeImpl::SetDisplay(uint8_t inverted)
{
   int32_t status;
   uint8_t data[8];

   GetPWFControllerInfo();
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
   flashUtil.UnlockController(m_deviceId, m_serialNumber);
#endif

   data[0] = 0xFF;
   data[1] = 0xFF;
   data[2] = 0xFF;
   data[3] = 0xFF;
   data[4] = 0xFF;
   data[5] = (inverted != 0);
   FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::SetNickname2 | m_sensorID, &data[0], 6, 0, &status);
}


void BattFuelGaugeImpl::ExecuteTask() {
   std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

   if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFastMsgRx).count() >= 10) {
      lastFastMsgRx = now;
      
      RxFastMsgs();
   }

   if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSlowMsgRx).count() >= 1000) {
      lastSlowMsgRx = now;
      
      RxSlowMsgs();
   }
}

void BattFuelGaugeImpl::RxFastMsgs() {
   static uint32_t lastTimestamp = 0;
   uint32_t timeStamp;
   uint8_t data[8];
   uint32_t messageId;
   uint8_t msgLength;
   int32_t status;

   // Power ******************************************************************
   messageId = (int)CANMsgApiId::Power | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_current_A = (double)(int32_t)( ((uint32_t)data[3] << 24) |
                                       ((uint32_t)data[2] << 16) |
                                       ((uint32_t)data[1] << 8)  |
                                       ((uint32_t)data[0]) )      / 1000.0;

      m_voltage_V = (double)(uint16_t)(((uint16_t)data[5] << 8) |
                                       ((uint16_t)data[4]) )       / 1000.0;

      m_dod_Ah   = (double)(int16_t)(  ((uint16_t)data[7] << 8) |
                                        ((uint16_t)data[6]) )       / 1000.0;

      if (timeStamp != lastTimestamp)
        {
            lastTimestamp = timeStamp;
            lastPowerMsgRx = std::chrono::steady_clock::now();
        }
   }

   // StateOfCharge **********************************************************
   messageId = (int)CANMsgApiId::StateOfCharge | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_soc_Ah   = (double)(int16_t)(  ((uint16_t)data[1] << 8) |
                                       ((uint16_t)data[0]) )       / 1000.0;

      m_effectiveCapacity_Ah = (double)(int16_t)(
                                       ((uint16_t)data[3] << 8) |
                                       ((uint16_t)data[2]) )       / 1000.0;

      m_chargeState = (BattFuelGauge_ChargeState)data[4];

      m_dod_Wh = (double)(int32_t)(    ((uint32_t)data[7] << 16) |
                                       ((uint32_t)data[6] << 8)  |
                                       ((uint32_t)data[5]) )      / 3600.0;
   }
}

void BattFuelGaugeImpl::RxSlowMsgs() {
   uint32_t timeStamp;
   uint8_t data[8];
   uint32_t messageId;
   uint8_t msgLength;
   int32_t status;

   // Cycle Current ***********************************************************
   messageId = (int)CANMsgApiId::CycleCurrent | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_cycleStats.minCurrent_A = (double)(int32_t)( 
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) )      / 1000.0;

      m_cycleStats.maxCurrent_A = (double)(int32_t)( 
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) )      / 1000.0;
   }

   // Cycle Charge ************************************************************
   messageId = (int)CANMsgApiId::CycleCharge | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_cycleStats.minVoltage_V = (double)(uint16_t)( 
         ((uint16_t)data[1] << 8) |
         ((uint16_t)data[0]) )       / 1000.0;

      m_cycleStats.maxVoltage_V = (double)(uint16_t)( 
         ((uint16_t)data[3] << 8) |
         ((uint16_t)data[2]) )       / 1000.0;

      m_cycleStats.minDOD_Ah = (double)(int16_t)( 
         ((uint16_t)data[5] << 8) |
         ((uint16_t)data[4]) )       / 1000.0;

      m_cycleStats.maxDOD_Ah = (double)(int16_t)( 
         ((uint16_t)data[7] << 8) |
         ((uint16_t)data[6]) )       / 1000.0;
   }

   // Cycle Time **************************************************************
   messageId = (int)CANMsgApiId::CycleTime | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_cycleStats.chargeTime_s = (
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) ) / 1000;

      m_cycleStats.dischargeTime_s = (
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) ) / 1000;
   }

   // Battery Health **********************************************************
   messageId = (int)CANMsgApiId::BatteryHealth | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_battAge_s = (
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) );

      m_capacity_Ah = (double)(uint16_t)( 
         ((uint16_t)data[5] << 8) |
         ((uint16_t)data[4]) )       / 1000.0;

      m_battNumCycles = (
         ((uint16_t)data[7] << 8) |
         ((uint16_t)data[6]) );
   }

   // Match Current ***********************************************************
   messageId = (int)CANMsgApiId::MatchCurrent | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_matchStats.minCurrent_A = (double)(int32_t)( 
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) )      / 1000.0;

      m_matchStats.maxCurrent_A = (double)(int32_t)( 
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) )      / 1000.0;
   }

   // Match Charge ************************************************************
   messageId = (int)CANMsgApiId::MatchCharge | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_matchStats.minVoltage_V = (double)(uint16_t)( 
         ((uint16_t)data[1] << 8) |
         ((uint16_t)data[0]) )       / 1000.0;

      m_matchStats.maxVoltage_V = (double)(uint16_t)( 
         ((uint16_t)data[3] << 8) |
         ((uint16_t)data[2]) )       / 1000.0;

      m_matchStats.minDOD_Ah = (double)(int16_t)( 
         ((uint16_t)data[5] << 8) |
         ((uint16_t)data[4]) )       / 1000.0;

      m_matchStats.maxDOD_Ah = (double)(int16_t)( 
         ((uint16_t)data[7] << 8) |
         ((uint16_t)data[6]) )       / 1000.0;
   }

   // Match Time **************************************************************
   messageId = (int)CANMsgApiId::MatchTime | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_matchStats.chargeTime_s = (
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) ) / 1000;

      m_matchStats.dischargeTime_s = (
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) ) / 1000;
   }

   // RMS Current *************************************************************
   messageId = (int)CANMsgApiId::RMSCurrent | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_matchStats.rmsDischargeCurrent_A = (double)(uint32_t)( 
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) )      / 1000.0;

      m_cycleStats.rmsDischargeCurrent_A = (double)(uint32_t)( 
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) )      / 1000.0;
   }

   // Calibration *************************************************************
   messageId = (int)CANMsgApiId::Calibration | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_measCurrentOffset = (
         ((uint32_t)data[3] << 24) |
         ((uint32_t)data[2] << 16) |
         ((uint32_t)data[1] << 8)  |
         ((uint32_t)data[0]) );

      m_measCurrentGain = (
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) );
   }

   // Cycle Energy ************************************************************
   messageId = (int)CANMsgApiId::CycleEnergy | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_cycleStats.minDOD_Wh = (double)(
         (  (int32_t)(int16_t)(uint16_t)(((uint16_t)data[1] << 8) | ((uint16_t)data[0]))  ) << 7) / 3600.0;

      m_cycleStats.maxDOD_Wh = (double)(
         (  (int32_t)(int16_t)(uint16_t)(((uint16_t)data[3] << 8) | ((uint16_t)data[2]))  ) << 7) / 3600.0;

      m_cycleStats.startingVoltage_V = (double)(uint16_t)( 
         ((uint16_t)data[5] << 8) |
         ((uint16_t)data[4]     ) ) / 1000.0;

      m_ratedCapacity_Ah = (double)(uint16_t)(
         ((uint16_t)data[7] << 8) |
         ((uint16_t)data[6]     ) ) / 1000.0;
   }

   // Match Delta Energy ******************************************************
   messageId = (int)CANMsgApiId::MatchDeltaEnergy | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_matchStats.startDOD_Wh = (double)(
         (  (int32_t)(int16_t)(uint16_t)(((uint16_t)data[1] << 8) | ((uint16_t)data[0]))  ) << 7) / 3600.0;

      m_matchStats.endDOD_Wh = (double)(
         (  (int32_t)(int16_t)(uint16_t)(((uint16_t)data[3] << 8) | ((uint16_t)data[2]))  ) << 7) / 3600.0;

      m_matchStats.startingVoltage_V = (double)(uint16_t)( 
         ((uint16_t)data[5] << 8) |
         ((uint16_t)data[4]) )       / 1000.0;

      m_battManufacturer = (BattFuelGauge_BattMfg)data[6];
      //TODO - 1Bytes
   }

   // Match Delta Charge ******************************************************
   messageId = (int)CANMsgApiId::MatchDeltaCharge | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_matchStats.startDOD_Ah = (double)(int16_t)(uint16_t)( 
         ((uint16_t)data[1] << 8) |
         ((uint16_t)data[0]) )       / 1000.0;

      m_matchStats.endDOD_Ah = (double)(int16_t)(uint16_t)( 
         ((uint16_t)data[3] << 8) |
         ((uint16_t)data[2]) )       / 1000.0;

      m_matchStats.activeTime_s = (
         ((uint32_t)data[7] << 24) |
         ((uint32_t)data[6] << 16) |
         ((uint32_t)data[5] << 8)  |
         ((uint32_t)data[4]) )       / 1000;
   }

   // Nickname 1 **************************************************************
   messageId = (int)CANMsgApiId::Nickname1 | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_szNickname[0] = data[0];
      m_szNickname[1] = data[1];
      m_szNickname[2] = data[2];
      m_szNickname[3] = data[3];
      m_szNickname[4] = data[4];
      m_szNickname[5] = data[5];
      m_szNickname[6] = data[6];
      m_szNickname[7] = data[7];
   }

   // Nickname 2 **************************************************************
   messageId = (int)CANMsgApiId::Nickname2 | m_sensorID;
   FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, m_sensorIdMask, &data[0], &msgLength, &timeStamp, &status);
   if ((status==0) && (msgLength == 8))
   {
      m_szNickname[8]  = data[0];
      m_szNickname[9]  = data[1];
      m_szNickname[10] = data[2];
      m_szNickname[11] = 0;

      // TODO 5 bytes
   }
}
