#include <stdlib.h>
#include "TimeOfFlightImpl.h"
#include "TimeOfFlightApi.h"
#include <map>

static std::map<uint8_t, TimeOfFlight_t*> handleList;

struct TimeOfFlight_s {
    void *obj;
};

TimeOfFlight_t *TimeOfFlight_Create(uint8_t sensorID) {
    TimeOfFlight_t *pHandle = NULL;

#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
    pwf::TimeOfFlightImpl *obj;

    if (handleList.find(sensorID) != handleList.end()) {
        pHandle = handleList[sensorID];
    }
    else {   
        pHandle  = (TimeOfFlight_t*)malloc( sizeof(*pHandle) );
        obj = new pwf::TimeOfFlightImpl(sensorID);
        pHandle->obj = obj;

        handleList[sensorID] = pHandle;
    }
#endif // __FRC_ROBORIO__
    return pHandle;
}

void TimeOfFlight_Destroy(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        
        handleList.erase( obj->GetSensorID() );

        delete obj;
        free(pHandle);
    }
}

void TimeOfFlight_IdentifySensor(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->IdentifySensor();
    }
}

uint32_t TimeOfFlight_GetFirmwareVersion(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    uint32_t version = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        version = obj->GetFirmwareVersion();
    }
    return version;
}

uint32_t TimeOfFlight_GetSerialNumber(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    uint32_t serial = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        serial = obj->GetSerialNumber();
    }
    return serial;
}

uint8_t TimeOfFlight_IsRangeValid(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    uint8_t isValid = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        isValid = obj->IsRangeValid();
    }
    return isValid;
}

double TimeOfFlight_GetRange(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    double range = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        range = obj->GetRange();
    }
    return range;
}

double TimeOfFlight_GetRangeSigma(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    double sigma = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        sigma = obj->GetRangeSigma();
    }
    return sigma;
}

double TimeOfFlight_GetAmbientLightLevel(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    double ambientLevel = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        ambientLevel = obj->GetAmbientLightLevel();
    }
    return ambientLevel;
}

TimeOfFlight_Status TimeOfFlight_GetStatus(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    TimeOfFlight_Status status = TimeOfFlight_Status::TimeOfFlight_Status_Invalid;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->ExecuteTask();
        status = obj->GetStatus();
    }
    return status;
}

TimeOfFlight_RangingMode TimeOfFlight_GetRangingMode(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    TimeOfFlight_RangingMode mode = TimeOfFlight_RangingMode::TimeOfFlight_RangingMode_kShort;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        mode = obj->GetRangingMode();
    }
    return mode;
}


double TimeOfFlight_GetSampleTime(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    double time = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        time = obj->GetSampleTime();
    }
    return time;
}

void TimeOfFlight_SetRangingMode(TimeOfFlight_t *pHandle, TimeOfFlight_RangingMode mode, double sampleTime) {
    pwf::TimeOfFlightImpl *obj;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->SetRangingMode(mode, sampleTime);
    }
}


void TimeOfFlight_SetRangeOfInterest(TimeOfFlight_t *pHandle, uint8_t topLeftX, uint8_t topLeftY, uint8_t bottomRightX, uint8_t bottomRightY) {
    pwf::TimeOfFlightImpl *obj;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        obj->SetRangeOfInterest(topLeftX, topLeftY, bottomRightX, bottomRightY);
    }
}

uint8_t TimeOfFlight_IsConnected(TimeOfFlight_t *pHandle) {
    pwf::TimeOfFlightImpl *obj;
    uint8_t isConnected = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::TimeOfFlightImpl *>(pHandle->obj);
        isConnected = obj->IsConnected();
    }

    return isConnected;
}