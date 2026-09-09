#include "jni.h"
#include "com_playingwithfusion_jni_CANVenomJNI.h"
#include "BattFuelGaugeApi.h"

extern "C" {

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_create(JNIEnv *, jclass, jint id) {
    BattFuelGauge_t *pHandle;

    pHandle = BattFuelGauge_Create((uint8_t)id);
    return (jlong)pHandle;
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_destroy(JNIEnv *, jclass, jlong handle) {
    BattFuelGauge_Destroy((BattFuelGauge_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_identify(JNIEnv *, jclass, jlong handle) {
    BattFuelGauge_Identify((BattFuelGauge_t*)handle);
}

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getFirmwareVersion(JNIEnv *, jclass, jlong handle) {
    return (jlong)BattFuelGauge_GetFirmwareVersion((BattFuelGauge_t*)handle);
}

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getSerialNumber(JNIEnv *, jclass, jlong handle) {
    return (jlong)BattFuelGauge_GetSerialNumber((BattFuelGauge_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_saveLog(
    JNIEnv* env, jclass clazz, jlong handle, jstring filename) {

    const char *pFilename = env->GetStringUTFChars(filename, NULL);
    BattFuelGauge_SaveLog((BattFuelGauge_t*)handle, pFilename);
}

JNIEXPORT jboolean JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_isConnected(JNIEnv *, jclass, jlong handle) {
    return (jboolean)BattFuelGauge_IsConnected((BattFuelGauge_t*)handle);
}

// ──────────────────────────────────────────────────────────────────────────────
// Core real-time / state values
// ──────────────────────────────────────────────────────────────────────────────

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getBatteryState(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jint) BattFuelGauge_GetBatteryState((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetVoltage((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getDodAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetDodAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getDodWh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetDodWh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getRemainingChargePct(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetRemainingChargePct((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getEffectiveCapacityAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetEffectiveCapacityAh((BattFuelGauge_t*)handle);
}

// ──────────────────────────────────────────────────────────────────────────────
// Battery age & identification
// ──────────────────────────────────────────────────────────────────────────────

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getBatteryAgeDays(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetBatteryAgeDays((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCapacityAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCapacityAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getRatedCapacityAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetRatedCapacityAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getNumCycles(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jint) BattFuelGauge_GetNumCycles((BattFuelGauge_t*)handle);
}

JNIEXPORT jstring JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getNickname(
    JNIEnv* env, jclass clazz, jlong handle) {
    const char* nick = BattFuelGauge_GetNickname((BattFuelGauge_t*)handle);
    return env->NewStringUTF(nick ? nick : "");
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getManufacturer(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jint) BattFuelGauge_GetManufacturer((BattFuelGauge_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_setNickname(
    JNIEnv* env, jclass clazz, jlong handle, jstring newNickname) {

    const char *pNewNickname = env->GetStringUTFChars(newNickname, NULL);
    BattFuelGauge_SetNickname((BattFuelGauge_t*)handle, pNewNickname);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_setManufacturer(
    JNIEnv* env, jclass clazz, jlong handle, jint newMfg) {
    BattFuelGauge_SetManufacturer((BattFuelGauge_t*)handle, (BattFuelGauge_BattMfg)newMfg);
}

// ──────────────────────────────────────────────────────────────────────────────
// Last charge cycle statistics
// ──────────────────────────────────────────────────────────────────────────────

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleRmsDischargeCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleRmsDischargeCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMaxCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMaxCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMinCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMinCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMinVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMinVoltage((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMaxVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMaxVoltage((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMinDodAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMinDodAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMaxDodAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMaxDodAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMinDodWh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMinDodWh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleMaxDodWh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleMaxDodWh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleChargeTime(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleChargeTime((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleDischargeTime(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleDischargeTime((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getCycleStartingVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetCycleStartingVoltage((BattFuelGauge_t*)handle);
}

// ──────────────────────────────────────────────────────────────────────────────
// Last robot match statistics
// ──────────────────────────────────────────────────────────────────────────────

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchRmsDischargeCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchRmsDischargeCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchMaxCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchMaxCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchMinCurrent(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchMinCurrent((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchMinVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchMinVoltage((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchMaxVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchMaxVoltage((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchMinDodAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchMinDodAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchMaxDodAh(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchMaxDodAh((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchChargeTime(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchChargeTime((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchDischargeTime(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchDischargeTime((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchStartingVoltage(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchStartingVoltage((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchStartingEnergy(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchStartingEnergy((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchEndingEnergy(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchEndingEnergy((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchStartingCharge(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchStartingCharge((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchEndingCharge(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchEndingCharge((BattFuelGauge_t*)handle);
}

JNIEXPORT jfloat JNICALL Java_com_playingwithfusion_jni_BattFuelGaugeJNI_getMatchDuration(
    JNIEnv* env, jclass clazz, jlong handle) {
    return (jfloat) BattFuelGauge_GetMatchDuration((BattFuelGauge_t*)handle);
}

}  // extern "C"