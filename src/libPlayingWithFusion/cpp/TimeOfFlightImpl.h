#pragma once

#include <cstdint>
#include <atomic>
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
  #include <FRC_NetworkCommunication/CANSessionMux.h>
#endif
#include "TimeOfFlightApiTypes.h"
#include "PeriodicTask.h"
#include "PeriodicHelper.h"

namespace pwf {

class TimeOfFlightImpl : PeriodicTask {
  public:
    explicit TimeOfFlightImpl(uint8_t sensorID);
    virtual ~TimeOfFlightImpl();

    // Delete Copy constructor and assignment operator
    TimeOfFlightImpl(const TimeOfFlightImpl&) =delete;
    TimeOfFlightImpl& operator=(const TimeOfFlightImpl&) =delete;

    // Use default Move constructor and assignment operator
    //TimeOfFlightImpl(TimeOfFlightImpl&&) =default;  //TBD: Mac compiler doesn't like this
    //TimeOfFlightImpl& operator=(TimeOfFlightImpl&&) =default;  //TBD: Mac compiler doesn't like this

    void     IdentifySensor();
    uint32_t GetFirmwareVersion() const;
    uint32_t GetSerialNumber() const;
    uint8_t  GetSensorID() const;

    bool   IsRangeValid() const;
    double GetRange() const;
    double GetRangeSigma() const;
    double GetAmbientLightLevel() const;
    TimeOfFlight_Status GetStatus() const;

    void SetRangeOfInterest(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY);
    void SetRangingMode(TimeOfFlight_RangingMode mode, double sampleTime);
    TimeOfFlight_RangingMode GetRangingMode() const;
    double GetSampleTime() const;

    bool IsConnected() const;

    void ExecuteTask() override;

  private:
    int m_sensorID;

    //PeriodicHelper periodicHelper;

    std::atomic<bool> m_isValid   = {false};
    std::atomic<double> m_range   = {0};
    std::atomic<double> m_sigma   = {0};
    std::atomic<double> m_ambient = {0};
    std::atomic<TimeOfFlight_Status> m_status = {TimeOfFlight_Status::TimeOfFlight_Status_Invalid};

    std::atomic<uint32_t> m_firmwareVersion = {0};
    std::atomic<uint32_t> m_serialNumber    = {0};

    std::atomic<int> m_topLeftX = {0};
    std::atomic<int> m_topLeftY = {0};
    std::atomic<int> m_bottomRightX = {15};
    std::atomic<int> m_bottomRightY = {15};

    std::atomic<double> m_sampleTime = {100};
    std::atomic<TimeOfFlight_RangingMode> m_rangingMode = {TimeOfFlight_RangingMode::TimeOfFlight_RangingMode_kShort};
    
    enum CANMsgApiId {
      Distance    = 0x060B0100,
      Quality     = 0x060B0200,
      CalState    = 0x060B0300,
      RangingCfg  = 0x060B0400,
      Heartbeat   = 0x1F0B0100,
      Reprog      = 0x1F0B03FF
    };

    std::chrono::steady_clock::time_point lastCfgMsgTx;
    std::chrono::steady_clock::time_point lastMsgRx;
    std::chrono::steady_clock::time_point lastDistanceMsgRx;

    void QueueCfgMsg(bool changed);

    double Clamp(double x, double min, double max);
    void TxCfgMsg();

    void RxDistanceMsg();
    void RxQualityMsg();
    void RxHeartBeatMsg();
};

}