#pragma once

#include <cstdint>
#include <string>
#include <atomic>
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
  #include <FRC_NetworkCommunication/CANSessionMux.h>
#endif
#include "BattFuelGaugeApiTypes.h"
#include "PeriodicTask.h"
#include "PeriodicHelper.h"
#include "PWFController.h"

namespace pwf {

class BattFuelGaugeImpl : PeriodicTask {
  public:
    explicit BattFuelGaugeImpl(uint8_t sensorID, uint8_t ignoreSensorID);
    virtual ~BattFuelGaugeImpl();

    // Delete Copy constructor and assignment operator
    BattFuelGaugeImpl(const BattFuelGaugeImpl&) =delete;
    BattFuelGaugeImpl& operator=(const BattFuelGaugeImpl&) =delete;

    // Use default Move constructor and assignment operator
    //BattFuelGaugeImpl(BattFuelGaugeImpl&&) =default;  //TBD: Mac compiler doesn't like this
    //BattFuelGaugeImpl& operator=(BattFuelGaugeImpl&&) =default;  //TBD: Mac compiler doesn't like this

    void     IdentifySensor();
    uint32_t GetFirmwareVersion();
    uint32_t GetSerialNumber();
    uint8_t  GetSensorID() const;

    std::string GetBfgStateJSON();
    void SaveLog(std::string filename = "");
    bool IsConnected() const;

    std::string GetChargeStateString() const;
    std::string GetManufacturerString() const;
    std::string GetNickname();

    void SetNickname(std::string name);
    void SetManufacturer(BattFuelGauge_BattMfg mfgId);

    void SetCalibrationVals(uint32_t currentOffset, uint32_t currentGain);
    void SetBatteryAge(double capacity_Ah, uint32_t age_s, uint16_t numCycles);
    void ResetAllBatteryStats();
    void InitiateCurrentOffsetCalibration();
    void InitiateCurrentGainCalibration();
    void SetDisplay(uint8_t inverted);

    void ExecuteTask() override;


    int m_sensorID;
    uint32_t m_sensorIdMask;

    uint32_t m_measCurrentOffset;
    uint32_t m_measCurrentGain;

    char m_szNickname[12] = "";
    BattFuelGauge_BattMfg m_battManufacturer = BattFuelGauge_BattMfg::BattFuelGauge_BattMfg_kMKPowered;
    uint32_t m_battAge_s      = 0;
    uint32_t m_battNumCycles  = 0;
    double m_ratedCapacity_Ah = 0;

    BattFuelGauge_ChargeState m_chargeState = BattFuelGauge_ChargeState::BattFuelGauge_ChargeState_kInit;
    double m_capacity_Ah          = 0;
    double m_effectiveCapacity_Ah = 0;
    double m_voltage_V            = 0;
    double m_current_A            = 0;
    double m_dod_Ah               = 0;
    double m_dod_Wh               = 0;
    double m_soc_Ah               = 0;

    BattFuelGauge_CycleStats m_cycleStats;
    BattFuelGauge_MatchStats m_matchStats;

    void RxFastMsgs();
    void RxSlowMsgs();

    void GetPWFControllerInfo();

    std::atomic<uint32_t> m_firmwareVersion = {0};
    std::atomic<uint32_t> m_serialNumber    = {0};
    std::atomic<pwf_device_t> m_deviceId    = {pwf_device_t::DEV_BFG_SENSOR};

  private:


    enum CANMsgApiId {
      Power               = 0x0A0B0000,
      BatteryHealth       = 0x0A0B0100,
      MatchCurrent        = 0x0A0B0200,
      MatchCharge         = 0x0A0B0300,
      MatchTime           = 0x0A0B0500,
      RMSCurrent          = 0x0A0B0600,
      StateOfCharge       = 0x0A0B0700,
      CycleCurrent        = 0x0A0B0800,
      CycleCharge         = 0x0A0B0900,
      CycleTime           = 0x0A0B0A00,
      Calibration         = 0x0A0B0B00,
      CycleEnergy         = 0x0A0B0C00,
      MatchDeltaEnergy    = 0x0A0B0D00,
      MatchDeltaCharge    = 0x0A0B0E00,
      Nickname1           = 0x0A0B0F00,
      Nickname2           = 0x0A0B1000,
      SetNickname1        = 0x0A0B2000,
      SetNickname2        = 0x0A0B2100,
      SetHealth           = 0x0A0B2200,
      SetCalibration      = 0x0A0B2300,
      Heartbeat           = 0x1F0B0100,
      Reprog              = 0x1F0B03FF,
    };

    std::chrono::steady_clock::time_point lastFastMsgRx;
    std::chrono::steady_clock::time_point lastSlowMsgRx;
    std::chrono::steady_clock::time_point lastPowerMsgRx;
};

}