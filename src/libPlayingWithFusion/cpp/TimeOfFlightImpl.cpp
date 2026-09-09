#include "TimeOfFlightImpl.h"
#include "math.h"
#include <chrono>
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
#include <FRC_NetworkCommunication/FRCComm.h>
#else
static void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t id, uint8_t *data, int y, int z, int32_t *status) {

}


static void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t *id, uint32_t mask, uint8_t *data, uint8_t *length, uint32_t *timeStamp, int32_t *status) {
    *length = 0;
}
#endif


using namespace pwf;

//HAL_CANManufacturer::HAL_CAN_Man_kTeamUse
TimeOfFlightImpl::TimeOfFlightImpl(uint8_t sensorID) {
    m_sensorID = sensorID;

  //  periodicHelper.AddTask(this);
}

TimeOfFlightImpl::~TimeOfFlightImpl() {
  //  periodicHelper.RemoveTask(this);
}

void TimeOfFlightImpl::IdentifySensor() {
    uint8_t data[6];
    int32_t status;

    data[0] = 0x0D;
    data[1] = m_serialNumber & 0xFF;
    data[2] = (m_serialNumber >> 8) & 0xFF;
    data[3] = (m_serialNumber >> 16) & 0xFF;
    data[4] = 0x10;
    data[5] = 0x01;

    FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::Reprog, &data[0], 6, 0, &status);
}

uint32_t TimeOfFlightImpl::GetFirmwareVersion() const {
    return m_firmwareVersion;
}

uint32_t TimeOfFlightImpl::GetSerialNumber() const {
    return m_serialNumber;
}

uint8_t TimeOfFlightImpl::GetSensorID() const {
    return m_sensorID;
}

bool TimeOfFlightImpl::IsRangeValid() const {
    return m_isValid;
}

double TimeOfFlightImpl::GetRange() const {
    return m_range;
}

double TimeOfFlightImpl::GetRangeSigma() const {
    return m_sigma;
}

double TimeOfFlightImpl::GetAmbientLightLevel() const {
    return m_ambient;
}

TimeOfFlight_Status TimeOfFlightImpl::GetStatus() const {
    return m_status;
}

void TimeOfFlightImpl::SetRangeOfInterest(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
    bool cfgChanged = (m_topLeftX != topLeftX) || (m_topLeftY != topLeftY) || (m_bottomRightX != bottomRightX) || (m_bottomRightY != bottomRightY);

    m_topLeftX = topLeftX;
    m_topLeftY = topLeftY;
    m_bottomRightX = bottomRightX;
    m_bottomRightY = bottomRightY;
    QueueCfgMsg(cfgChanged);
}

void TimeOfFlightImpl::SetRangingMode(TimeOfFlight_RangingMode mode, double sampleTime) {
    bool cfgChanged = (m_rangingMode != mode) || (m_sampleTime != sampleTime);

    m_rangingMode = mode;
    m_sampleTime  = sampleTime;
    QueueCfgMsg(cfgChanged);
}

TimeOfFlight_RangingMode TimeOfFlightImpl::GetRangingMode() const {
    return m_rangingMode;
}

double TimeOfFlightImpl::GetSampleTime() const {
    return m_sampleTime;
}

bool TimeOfFlightImpl::IsConnected() const {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    return (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDistanceMsgRx).count() < 2000);
}

void TimeOfFlightImpl::ExecuteTask() {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMsgRx).count() >= 10) {
        lastMsgRx = now;
        
        RxDistanceMsg();
        RxQualityMsg();
        RxHeartBeatMsg();

        QueueCfgMsg(false);
    }
}

void TimeOfFlightImpl::QueueCfgMsg(bool changed) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if (changed || std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCfgMsgTx).count() >= 400) {
        lastCfgMsgTx = now;
        TxCfgMsg();
    }
}

double TimeOfFlightImpl::Clamp(double x, double min, double max) {
    if (isnan(x)) {
        x = 0;
    }

    if (x > max) {
        x = max;
    }
    else if (x < min) {
        x = min;
    }

    return x;
}

void TimeOfFlightImpl::TxCfgMsg() {
    uint8_t data[7];
    uint16_t rawTime;
    int32_t status;

    rawTime  = (uint16_t)Clamp(m_sampleTime, 10.0, 999.0);

    data[0] = ((uint8_t)m_rangingMode) + 1;
    data[1] = (uint8_t)(rawTime & 0xFF);
    data[2] = (uint8_t)(rawTime >> 8);
    data[3] = (uint8_t)m_topLeftX;
    data[4] = (uint8_t)m_topLeftY;
    data[5] = (uint8_t)m_bottomRightX;
    data[6] = (uint8_t)m_bottomRightY;

    FRC_NetworkCommunication_CANSessionMux_sendMessage((int)CANMsgApiId::RangingCfg | m_sensorID, &data[0], 7, 0, &status);
}

void TimeOfFlightImpl::RxDistanceMsg() {
    static uint32_t lastTimestamp = 0;
    uint32_t timeStamp;
    uint8_t data[8];
    uint32_t messageId;
    uint8_t msgLength;
    int32_t status;

    messageId = (int)CANMsgApiId::Distance | m_sensorID;
    FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, 0x1FFFFFFF, &data[0], &msgLength, &timeStamp, &status);

    if ((status==0) && (msgLength == 8))
    {
        m_range = (double)(int16_t)( ((uint16_t)data[1] << 8) |
                                     ((uint16_t)data[0]) );

        m_status = (TimeOfFlight_Status)data[2];
        m_isValid = (m_status == TimeOfFlight_Status::TimeOfFlight_Status_Valid);

        if (timeStamp != lastTimestamp)
        {
            lastTimestamp = timeStamp;
            lastDistanceMsgRx = std::chrono::steady_clock::now();
        }
    }
}

void TimeOfFlightImpl::RxQualityMsg() {
    uint32_t timeStamp;
    uint8_t data[8];
    uint32_t messageId;
    uint8_t msgLength;
    int32_t status;

    messageId = (int)CANMsgApiId::Quality | m_sensorID;
    FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, 0x1FFFFFFF, &data[0], &msgLength, &timeStamp, &status);

    if ((status==0) && (msgLength == 8))
    {
        m_ambient = (double)( (int32_t)(((uint32_t)data[3] << 24) |
                                        ((uint32_t)data[2] << 16) |
                                        ((uint32_t)data[1] << 8)  |
                                        ((uint32_t)data[0])) ) / 65536.0f;

        m_sigma   = (double)( (int32_t)(((uint32_t)data[7] << 24) |
                                        ((uint32_t)data[6] << 16) |
                                        ((uint32_t)data[5] << 8)  |
                                        ((uint32_t)data[4])) ) / 65536.0f;
    }
}

void TimeOfFlightImpl::RxHeartBeatMsg() {
    uint32_t timeStamp;
    uint8_t data[8];
    uint32_t messageId;
    uint8_t msgLength;
    int32_t status;

    messageId = (int)CANMsgApiId::Heartbeat | m_sensorID;
    FRC_NetworkCommunication_CANSessionMux_receiveMessage(&messageId, 0x1FFFFFFF, &data[0], &msgLength, &timeStamp, &status);

    if ((status==0) && (msgLength == 8))
    {
        m_serialNumber = ((uint32_t)data[3] << 16) |
                         ((uint32_t)data[2] << 8)  |
                         ((uint32_t)data[1]);

        m_firmwareVersion = ((uint16_t)data[7] << 8) |
                            ((uint16_t)data[6]);
    }
}
