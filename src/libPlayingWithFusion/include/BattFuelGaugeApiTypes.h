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
      double minVoltage_V;
      double maxVoltage_V;
      double minCurrent_A;
      double maxCurrent_A;
      double startingVoltage_V;
      double rmsDischargeCurrent_A;
      double minDOD_Ah;
      double maxDOD_Ah;
      uint32_t chargeTime_s;
      uint32_t dischargeTime_s;
};

class BattFuelGauge_CycleStats : public BattFuelGauge_Stats
{
   public:
      double minDOD_Wh;
      double maxDOD_Wh;
};

class BattFuelGauge_MatchStats : public BattFuelGauge_Stats
{
   public:
      uint32_t activeTime_s;
      double startDOD_Ah;
      double startDOD_Wh;
      double endDOD_Ah;
      double endDOD_Wh;
};
