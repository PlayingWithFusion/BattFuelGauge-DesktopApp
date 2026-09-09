#pragma once

#include <cstdint>

typedef enum BattFuelGauge_ChargeState_e {
   BattFuelGauge_ChargeState_kInit           = 0,
   BattFuelGauge_ChargeState_kDischarge      = 1,
   BattFuelGauge_ChargeState_kConstICharge   = 2,
   BattFuelGauge_ChargeState_kConstVCharge   = 3,
   BattFuelGauge_ChargeState_kTrickleCharge  = 4,
   BattFuelGauge_ChargeState_kIdle           = 5,
   BattFuelGauge_ChargeState_kMeasureOCV     = 6
} BattFuelGauge_ChargeState;


typedef enum BattFuelGauge_BattMfg_e
{
   BattFuelGauge_BattMfg_kDuracell,
   BattFuelGauge_BattMfg_kEnergizer,
   BattFuelGauge_BattMfg_kInterstate,
   BattFuelGauge_BattMfg_kMightyMax,
   BattFuelGauge_BattMfg_kMKPowered,
   BattFuelGauge_BattMfg_kPowerSonic,
   BattFuelGauge_BattMfg_kNumMfgs
} BattFuelGauge_BattMfg;


class BattFuelGauge_Stats
{
   public:
      double minVoltage_V = 0;
      double maxVoltage_V = 0;
      double minCurrent_A = 0;
      double maxCurrent_A = 0;
      double startingVoltage_V = 0;
      double rmsDischargeCurrent_A = 0;
      double minDOD_Ah = 0;
      double maxDOD_Ah = 0;
      uint32_t chargeTime_s = 0;
      uint32_t dischargeTime_s = 0;
};

class BattFuelGauge_CycleStats : public BattFuelGauge_Stats
{
   public:
      double minDOD_Wh = 0;
      double maxDOD_Wh = 0;
};

class BattFuelGauge_MatchStats : public BattFuelGauge_Stats
{
   public:
      uint32_t activeTime_s = 0;
      double startDOD_Ah = 0;
      double startDOD_Wh = 0;
      double endDOD_Ah = 0;
      double endDOD_Wh = 0;
};
