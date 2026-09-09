#ifndef BATTFUELGAUGE_API_H
#define BATTFUELGAUGE_API_H

#include <stdint.h>
#include "BattFuelGaugeApiTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file BattFuelGaugeApi.h
 * @brief C-style API for accessing battery fuel gauge data.
 *
 * This header declares the public C-compatible interface functions used to
 * retrieve battery status, statistics, and historical data.
 *
 * All functions accept a pointer to a BattFuelGauge_t opaque handle.
 * If the handle is NULL, functions should return safe default values
 * (typically 0.0f, 0, or empty string).
 */

struct BattFuelGauge_s;
typedef struct BattFuelGauge_s BattFuelGauge_t;


BattFuelGauge_t *BattFuelGauge_Create(uint8_t id);
void     BattFuelGauge_Destroy(BattFuelGauge_t *pHandle);

void     BattFuelGauge_Identify(BattFuelGauge_t *pHandle);
uint32_t BattFuelGauge_GetFirmwareVersion(BattFuelGauge_t *pHandle);
uint32_t BattFuelGauge_GetSerialNumber(BattFuelGauge_t *pHandle);

void BattFuelGauge_SaveLog(BattFuelGauge_t *pHandle, const char* pFilename);

/**
 * @brief Determine if sensor is present on the CAN bus and at least one CAN message
 *        was received the past two seconds
 * 
 * @return true if at least on CAN message was received from sensor within the past two seconds, otherwise false
 */
uint8_t BattFuelGauge_IsConnected(BattFuelGauge_t *pHandle);


// -----------------------------------------------------------------------------
// Core real-time / state values
// -----------------------------------------------------------------------------

/**
 * @brief Get the battery state.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Integer code/enum value representing the battery state
 */
BattFuelGauge_ChargeState BattFuelGauge_GetBatteryState(BattFuelGauge_t *pHandle);

/**
 * @brief Get the battery discharge current.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Battery discharge current in Amperes (A)
 */
float BattFuelGauge_GetCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Get the current battery terminal voltage.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Battery voltage in Volts (V)
 */
float BattFuelGauge_GetVoltage(BattFuelGauge_t *pHandle);

/**
 * @brief Get the current depth of discharge in ampere-hours.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Depth of discharge in Ah
 */
float BattFuelGauge_GetDodAh(BattFuelGauge_t *pHandle);

/**
 * @brief Get the current depth of discharge energy.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Depth of discharge energy in Watt-hours (Wh)
 */
float BattFuelGauge_GetDodWh(BattFuelGauge_t *pHandle);

/**
 * @brief Get the estimated remaining charge percentage.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Remaining charge in percent (typically 0–100)
 */
float BattFuelGauge_GetRemainingChargePct(BattFuelGauge_t *pHandle);

/**
 * @brief Get the effective (usable) capacity of the battery.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Effective capacity in Ah
 */
float BattFuelGauge_GetEffectiveCapacityAh(BattFuelGauge_t *pHandle);

// -----------------------------------------------------------------------------
// Battery age, capacity & cycle information
// -----------------------------------------------------------------------------

/**
 * @brief Get the age of the battery.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Battery age in days
 */
float BattFuelGauge_GetBatteryAgeDays(BattFuelGauge_t *pHandle);

/**
 * @brief Get the current estimated battery capacity.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Battery capacity in Ah
 */
float BattFuelGauge_GetCapacityAh(BattFuelGauge_t *pHandle);

/**
 * @brief Get the original rated capacity when the battery was new.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Rated capacity in Ah
 */
float BattFuelGauge_GetRatedCapacityAh(BattFuelGauge_t *pHandle);

/**
 * @brief Get the total number of charge/discharge cycles.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Number of charge/discharge cycles
 */
uint32_t BattFuelGauge_GetNumCycles(BattFuelGauge_t *pHandle);

// -----------------------------------------------------------------------------
// Identification
// -----------------------------------------------------------------------------

/**
 * @brief Get the user-defined nickname of the battery.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Pointer to a null-terminated string containing the nickname,
 *         or empty string ("") if not set or unavailable.
 *         The returned pointer is valid only until the next API call or handle destruction.
 */
const char* BattFuelGauge_GetNickname(BattFuelGauge_t *pHandle);

/**
 * @brief Get the battery manufacturer identifier.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Integer code/enum value representing the manufacturer
 */
BattFuelGauge_BattMfg BattFuelGauge_GetManufacturer(BattFuelGauge_t *pHandle);

/**
 * @brief Set the user-defined nickname of the battery.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @param pNewNickname Pointer to a null-terminated string containing the new nickname
 * @return None
 */
void BattFuelGauge_SetNickname(BattFuelGauge_t *pHandle, const char* pNewNickname);

/**
 * @brief Set the battery manufacturer identifier.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @param newMfg Enum value representing the manufacturer
 * @return None
 */
void BattFuelGauge_SetManufacturer(BattFuelGauge_t *pHandle, BattFuelGauge_BattMfg newMfg);


// -----------------------------------------------------------------------------
// Last full charge cycle statistics
// -----------------------------------------------------------------------------

/**
 * @brief RMS discharge current during the last complete charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return RMS discharge current in Amps (A)
 */
float BattFuelGauge_GetCycleRmsDischargeCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum charging current observed during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum charging current in Amps (A)
 */
float BattFuelGauge_GetCycleMaxCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum (most negative) current observed during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum current in Amps (A)
 */
float BattFuelGauge_GetCycleMinCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum voltage observed during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum voltage in Volts (V)
 */
float BattFuelGauge_GetCycleMinVoltage(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum voltage observed during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum voltage in Volts (V)
 */
float BattFuelGauge_GetCycleMaxVoltage(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum depth of discharge reached during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum depth of discharge in Ah
 */
float BattFuelGauge_GetCycleMinDodAh(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum depth of discharge reached during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum depth of discharge in Ah
 */
float BattFuelGauge_GetCycleMaxDodAh(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum energy discharged during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum depth of discharge energy in Wh
 */
float BattFuelGauge_GetCycleMinDodWh(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum energy discharged during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum depth of discharge energy in Wh
 */
float BattFuelGauge_GetCycleMaxDodWh(BattFuelGauge_t *pHandle);

/**
 * @brief Total time spent charging during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Charging time in seconds
 */
float BattFuelGauge_GetCycleChargeTime(BattFuelGauge_t *pHandle);

/**
 * @brief Total time spent discharging during the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Discharging time in seconds
 */
float BattFuelGauge_GetCycleDischargeTime(BattFuelGauge_t *pHandle);

/**
 * @brief Starting voltage at the beginning of the last charge cycle.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Starting voltage in Volts (V)
 */
float BattFuelGauge_GetCycleStartingVoltage(BattFuelGauge_t *pHandle);

// -----------------------------------------------------------------------------
// Last robot match statistics
// -----------------------------------------------------------------------------

/**
 * @brief RMS discharge current during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return RMS discharge current in Amps (A)
 */
float BattFuelGauge_GetMatchRmsDischargeCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum current (any direction) during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum current in Amps (A)
 */
float BattFuelGauge_GetMatchMaxCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum (most negative discharge) current during the last match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum current in Amps (A)
 */
float BattFuelGauge_GetMatchMinCurrent(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum voltage observed during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum voltage in Volts (V)
 */
float BattFuelGauge_GetMatchMinVoltage(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum voltage observed during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum voltage in Volts (V)
 */
float BattFuelGauge_GetMatchMaxVoltage(BattFuelGauge_t *pHandle);

/**
 * @brief Minimum depth of discharge reached during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Minimum depth of discharge in Ah
 */
float BattFuelGauge_GetMatchMinDodAh(BattFuelGauge_t *pHandle);

/**
 * @brief Maximum depth of discharge reached during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Maximum depth of discharge in Ah
 */
float BattFuelGauge_GetMatchMaxDodAh(BattFuelGauge_t *pHandle);

/**
 * @brief Total charging time during the last robot match (if charging occurred).
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Charging time in seconds
 */
float BattFuelGauge_GetMatchChargeTime(BattFuelGauge_t *pHandle);

/**
 * @brief Total discharging time during the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Discharging time in seconds
 */
float BattFuelGauge_GetMatchDischargeTime(BattFuelGauge_t *pHandle);

/**
 * @brief Battery voltage at the start of the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Starting voltage in Volts (V)
 */
float BattFuelGauge_GetMatchStartingVoltage(BattFuelGauge_t *pHandle);

/**
 * @brief Energy stored at the beginning of the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Starting energy in Watt-hours (Wh)
 */
float BattFuelGauge_GetMatchStartingEnergy(BattFuelGauge_t *pHandle);

/**
 * @brief Energy remaining at the end of the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Ending energy in Watt-hours (Wh)
 */
float BattFuelGauge_GetMatchEndingEnergy(BattFuelGauge_t *pHandle);

/**
 * @brief Charge stored at the beginning of the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Starting charge in Ampere-hours (Ah)
 */
float BattFuelGauge_GetMatchStartingCharge(BattFuelGauge_t *pHandle);

/**
 * @brief Charge remaining at the end of the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Ending charge in Ampere-hours (Ah)
 */
float BattFuelGauge_GetMatchEndingCharge(BattFuelGauge_t *pHandle);

/**
 * @brief Duration of the last robot match.
 * @param pHandle Pointer to the BattFuelGauge instance handle
 * @return Match duration in seconds
 */
float BattFuelGauge_GetMatchDuration(BattFuelGauge_t *pHandle);

#ifdef __cplusplus
}
#endif

#endif // BATTFUELGAUGE_API_H