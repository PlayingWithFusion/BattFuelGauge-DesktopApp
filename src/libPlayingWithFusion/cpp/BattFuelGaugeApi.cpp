#include <stdlib.h>
#include "BattFuelGaugeImpl.h"
#include "BattFuelGaugeApi.h"
#include "FusionFlashUtil.h"
#include "FusionHttpServer.h"
#include <map>

static std::map<uint8_t, BattFuelGauge_t*> handleList;

#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
extern pwf::FusionFlashUtil flashUtil;
extern pwf::FusionHttpServer httpServer;
#endif // __FRC_ROBORIO__


struct BattFuelGauge_s {
    void *obj;
};


namespace pwf {
    class BattFuelGaugeImpl;
}


BattFuelGauge_t *BattFuelGauge_Create(uint8_t id) {
    BattFuelGauge_t *pHandle = NULL;
#if defined(__FRC_ROBORIO__) || defined(WIN32_PCAN)
    pwf::BattFuelGaugeImpl *obj = NULL;

    if (handleList.find(id) != handleList.end()) {
        pHandle = handleList[id];
    }
    else {   
        pHandle  = (BattFuelGauge_t*)malloc( sizeof(*pHandle) );
        obj = new pwf::BattFuelGaugeImpl(id, 1);
        pHandle->obj = obj;
        handleList[id] = pHandle;
    }
#endif // __FRC_ROBORIO__

    return pHandle;
}

void BattFuelGauge_Destroy(BattFuelGauge_t *pHandle) {
    pwf::BattFuelGaugeImpl *obj;

    if (pHandle != NULL) {
        obj = static_cast<pwf::BattFuelGaugeImpl *>(pHandle->obj);
        
        handleList.erase( obj->GetSensorID() );

        delete obj;
        free(pHandle);
    }
}

void BattFuelGauge_Identify(BattFuelGauge_t *pHandle) {
    pwf::BattFuelGaugeImpl *obj;

    if (pHandle != NULL) {
        obj = static_cast<pwf::BattFuelGaugeImpl *>(pHandle->obj);
        obj->IdentifySensor();
    }
}

uint32_t BattFuelGauge_GetFirmwareVersion(BattFuelGauge_t *pHandle) {
    pwf::BattFuelGaugeImpl *obj;
    uint32_t version = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::BattFuelGaugeImpl *>(pHandle->obj);
        version = obj->GetFirmwareVersion();
    }
    return version;
}

uint32_t BattFuelGauge_GetSerialNumber(BattFuelGauge_t *pHandle) {
    pwf::BattFuelGaugeImpl *obj;
    uint32_t serial = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::BattFuelGaugeImpl *>(pHandle->obj);
        serial = obj->GetSerialNumber();
    }
    return serial;
}

void BattFuelGauge_SaveLog(BattFuelGauge_t *pHandle, const char* pFilename) {
    if (pHandle == nullptr) return;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    std::string filename(pFilename); 
    obj->ExecuteTask();
    obj->SaveLog(filename);
}

uint8_t BattFuelGauge_IsConnected(BattFuelGauge_t *pHandle) {
    pwf::BattFuelGaugeImpl *obj;
    uint8_t isConnected = 0;

    if (pHandle != NULL) {
        obj = static_cast<pwf::BattFuelGaugeImpl *>(pHandle->obj);
        isConnected = obj->IsConnected();
    }

    return isConnected;
}

// ──────────────────────────────────────────────────────────────────────────────
// Core real-time / state values
// ──────────────────────────────────────────────────────────────────────────────

BattFuelGauge_ChargeState BattFuelGauge_GetBatteryState(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return BattFuelGauge_ChargeState::BattFuelGauge_ChargeState_kInit;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_chargeState;
}

float BattFuelGauge_GetCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_current_A;
}

float BattFuelGauge_GetVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_voltage_V;
}

float BattFuelGauge_GetDodAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_dod_Ah;
}

float BattFuelGauge_GetDodWh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_dod_Wh;
}

float BattFuelGauge_GetRemainingChargePct(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();

    float socPct = 0.0f;
    if (obj->m_capacity_Ah > 0)
    {
        socPct = 100.0f * obj->m_soc_Ah / obj->m_capacity_Ah;
    }

    return socPct;
}

float BattFuelGauge_GetEffectiveCapacityAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_effectiveCapacity_Ah;
}

// ──────────────────────────────────────────────────────────────────────────────
// Battery age & identification
// ──────────────────────────────────────────────────────────────────────────────

float BattFuelGauge_GetBatteryAgeDays(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return (float)obj->m_battAge_s / (3600.0 * 24.0);
}

float BattFuelGauge_GetCapacityAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_capacity_Ah;
}

float BattFuelGauge_GetRatedCapacityAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_ratedCapacity_Ah;
}

uint32_t BattFuelGauge_GetNumCycles(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_battNumCycles;
}

const char* BattFuelGauge_GetNickname(BattFuelGauge_t *pHandle) {
    static char szBuffer[12];
    
    if (pHandle == nullptr) return "";
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();

    std::size_t len = obj->GetNickname().copy(szBuffer, sizeof(szBuffer)-1);
    szBuffer[len] = 0;

    return szBuffer;
}

BattFuelGauge_BattMfg BattFuelGauge_GetManufacturer(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return BattFuelGauge_BattMfg::BattFuelGauge_BattMfg_kNumMfgs;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_battManufacturer;
}

void BattFuelGauge_SetNickname(BattFuelGauge_t *pHandle, const char* pNewNickname) {
    if (pHandle == nullptr) return;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    std::string newNickname(pNewNickname); 
    obj->SetNickname(newNickname);
}

void BattFuelGauge_SetManufacturer(BattFuelGauge_t *pHandle, BattFuelGauge_BattMfg newMfg) {
    if (pHandle == nullptr) return;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->SetManufacturer(newMfg);
}


// ──────────────────────────────────────────────────────────────────────────────
// Last charge cycle statistics
// ──────────────────────────────────────────────────────────────────────────────

float BattFuelGauge_GetCycleRmsDischargeCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.rmsDischargeCurrent_A;
}

float BattFuelGauge_GetCycleMaxCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.maxCurrent_A;
}

float BattFuelGauge_GetCycleMinCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.minCurrent_A;
}

float BattFuelGauge_GetCycleMinVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.minVoltage_V;
}

float BattFuelGauge_GetCycleMaxVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.maxVoltage_V;
}

float BattFuelGauge_GetCycleMinDodAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.minDOD_Ah;
}

float BattFuelGauge_GetCycleMaxDodAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.maxDOD_Ah;
}

float BattFuelGauge_GetCycleMinDodWh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.minDOD_Ah;
}

float BattFuelGauge_GetCycleMaxDodWh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.maxDOD_Wh;
}

float BattFuelGauge_GetCycleChargeTime(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.chargeTime_s;
}

float BattFuelGauge_GetCycleDischargeTime(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.dischargeTime_s;
}

float BattFuelGauge_GetCycleStartingVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_cycleStats.startingVoltage_V;
}

// ──────────────────────────────────────────────────────────────────────────────
// Last robot match statistics
// ──────────────────────────────────────────────────────────────────────────────

float BattFuelGauge_GetMatchRmsDischargeCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.rmsDischargeCurrent_A;
}

float BattFuelGauge_GetMatchMaxCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.maxCurrent_A;
}

float BattFuelGauge_GetMatchMinCurrent(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.minCurrent_A;
}

float BattFuelGauge_GetMatchMinVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.minVoltage_V;
}

float BattFuelGauge_GetMatchMaxVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.maxVoltage_V;
}

float BattFuelGauge_GetMatchMinDodAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.minDOD_Ah;
}

float BattFuelGauge_GetMatchMaxDodAh(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.maxDOD_Ah;
}

float BattFuelGauge_GetMatchChargeTime(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.chargeTime_s;
}

float BattFuelGauge_GetMatchDischargeTime(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.dischargeTime_s;
}

float BattFuelGauge_GetMatchStartingVoltage(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.startingVoltage_V;
}

float BattFuelGauge_GetMatchStartingEnergy(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.startDOD_Wh;
}

float BattFuelGauge_GetMatchEndingEnergy(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.endDOD_Wh;
}

float BattFuelGauge_GetMatchStartingCharge(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.startDOD_Ah;
}

float BattFuelGauge_GetMatchEndingCharge(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.endDOD_Ah;
}

float BattFuelGauge_GetMatchDuration(BattFuelGauge_t *pHandle) {
    if (pHandle == nullptr) return 0.0f;
    auto* obj = static_cast<pwf::BattFuelGaugeImpl*>(pHandle->obj);
    obj->ExecuteTask();
    return obj->m_matchStats.activeTime_s;
}
