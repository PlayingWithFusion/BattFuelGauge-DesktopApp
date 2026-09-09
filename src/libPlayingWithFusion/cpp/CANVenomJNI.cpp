#include "jni.h"
#include "com_playingwithfusion_jni_CANVenomJNI.h"
#include "CANVenomApi.h"

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_create(JNIEnv *, jclass, jint motorId) {
    CANVenom_t *pHandle;

    pHandle = CANVenom_Create((uint8_t)motorId);
    return (jlong)pHandle;
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_destroy(JNIEnv *, jclass, jlong handle) {
    CANVenom_Destroy((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_identifyMotor(JNIEnv *, jclass, jlong handle) {
    CANVenom_IdentifyMotor((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_resetPosition(JNIEnv *, jclass, jlong handle) {
    CANVenom_ResetPosition((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setPosition(JNIEnv *, jclass, jlong handle, jdouble newPosition) {
    CANVenom_SetPosition((CANVenom_t*)handle, (double)newPosition);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_stopMotor(JNIEnv *, jclass, jlong handle) {
    CANVenom_StopMotor((CANVenom_t*)handle);
}

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getFirmwareVersion(JNIEnv *, jclass, jlong handle) {
    return (jlong)CANVenom_GetFirmwareVersion((CANVenom_t*)handle);
}

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getSerialNumber(JNIEnv *, jclass, jlong handle) {
    return (jlong)CANVenom_GetSerialNumber((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_set(JNIEnv *, jclass, jlong handle, jdouble command) {
    CANVenom_Set((CANVenom_t*)handle, (double)command);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_pidWrite(JNIEnv *, jclass, jlong handle, jdouble command) {
    CANVenom_PIDWrite((CANVenom_t*)handle, (double)command);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setInverted(JNIEnv *, jclass, jlong handle, jboolean isInverted) {
    CANVenom_SetInverted((CANVenom_t*)handle, (uint8_t)isInverted);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_get(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_Get((CANVenom_t*)handle);
}

JNIEXPORT jboolean JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getInverted(JNIEnv *, jclass, jlong handle) {
    return (jboolean)CANVenom_GetInverted((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_disable(JNIEnv *, jclass, jlong handle) {
    CANVenom_Disable((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_enable(JNIEnv *, jclass, jlong handle) {
    CANVenom_Enable((CANVenom_t*)handle);
}

JNIEXPORT jboolean JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getFwdLimitSwitchActive(JNIEnv *, jclass, jlong handle) {
    return (jboolean)CANVenom_GetFwdLimitSwitchActive((CANVenom_t*)handle);
}

JNIEXPORT jboolean JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getRevLimitSwitchActive(JNIEnv *, jclass, jlong handle) {
    return (jboolean)CANVenom_GetRevLimitSwitchActive((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_enableLimitSwitches(JNIEnv *, jclass, jlong handle, jboolean fwdLimitSwitchEnabled, jboolean revLimitSwitchEnabled) {
    CANVenom_EnableLimitSwitches((CANVenom_t*)handle, (uint8_t)fwdLimitSwitchEnabled, (uint8_t)revLimitSwitchEnabled);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getNumAvaliableMotionProfilePoints(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetNumAvaliableMotionProfilePoints((CANVenom_t*)handle);
}

JNIEXPORT jint Java_com_playingwithfusion_jni_CANVenomJNI_getCurrentMotionProfilePoint(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetCurrentMotionProfilePoint((CANVenom_t*)handle);
}

JNIEXPORT jdouble Java_com_playingwithfusion_jni_CANVenomJNI_getMotionProfilePositionTarget(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMotionProfilePositionTarget((CANVenom_t*)handle);
}

JNIEXPORT jdouble Java_com_playingwithfusion_jni_CANVenomJNI_getMotionProfileSpeedTarget(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMotionProfileSpeedTarget((CANVenom_t*)handle);
}

JNIEXPORT jboolean Java_com_playingwithfusion_jni_CANVenomJNI_getMotionProfileIsValid(JNIEnv *, jclass, jlong handle) {
    return (jboolean)CANVenom_GetMotionProfileIsValid((CANVenom_t*)handle);
}

JNIEXPORT jint Java_com_playingwithfusion_jni_CANVenomJNI_getMotionProfileState(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetMotionProfileState((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_clearMotionProfilePoints(JNIEnv *, jclass, jlong handle) {
    CANVenom_ClearMotionProfilePoints((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_addMotionProfilePoint(JNIEnv *, jclass, jlong handle, jdouble time, jdouble speed, jdouble position) {
    CANVenom_AddMotionProfilePoint((CANVenom_t*)handle, (double)time, (double)speed, (double)position);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_completeMotionProfilePath(JNIEnv *, jclass, jlong handle, jdouble time, jdouble position) {
    CANVenom_CompleteMotionProfilePath((CANVenom_t*)handle, (double)time, (double)position);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_executePath(JNIEnv *, jclass, jlong handle) {
    CANVenom_ExecutePath((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getBusVoltage(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetBusVoltage((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getOutputVoltage(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetOutputVoltage((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getDutyCycle(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetDutyCycle((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getOutputCurrent(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetOutputCurrent((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getTemperature(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetTemperature((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getAuxVoltage(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetAuxVoltage((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getSpeed(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetSpeed((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getPosition(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetPosition((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getPIDTarget(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetPIDTarget((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getKF(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetKF((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getB(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetB((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getKP(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetKP((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getKI(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetKI((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getKD(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetKD((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getMinPILimit(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMinPILimit((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getMaxPILimit(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMaxPILimit((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getMaxSpeed(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMaxSpeed((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getMaxAcceleration(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMaxAcceleration((CANVenom_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getMaxJerk(JNIEnv *, jclass, jlong handle) {
    return (jdouble)CANVenom_GetMaxJerk((CANVenom_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getControlMode(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetControlMode((CANVenom_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getActiveControlMode(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetActiveControlMode((CANVenom_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getBrakeCoastMode(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetBrakeCoastMode((CANVenom_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getActiveFaults(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetActiveFaults((CANVenom_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_getLatchedFaults(JNIEnv *, jclass, jlong handle) {
    return (jint)CANVenom_GetLatchedFaults((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_clearLatchedFaults(JNIEnv *, jclass, jlong handle) {
    CANVenom_ClearLatchedFaults((CANVenom_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setCommandAndMode(JNIEnv *, jclass, jlong handle, jint mode, jdouble command) {
    CANVenom_SetCommandAndMode((CANVenom_t*)handle, (CANVenom_ControlMode)mode, (double)command);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setCommand(JNIEnv *, jclass, jlong handle, jint mode, jdouble command, jdouble kF, jdouble b) {
    CANVenom_SetCommand((CANVenom_t*)handle, (CANVenom_ControlMode)mode, (double)command, (double)kF, (double)b);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setPID(JNIEnv *, jclass, jlong handle, jdouble kP, jdouble kI, jdouble kD, jdouble kF, jdouble b) {
    CANVenom_SetPID((CANVenom_t*)handle, (double)kP, (double)kI, (double)kD, (double)kF, (double)b);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setKF(JNIEnv *, jclass, jlong handle, jdouble kF) {
    CANVenom_SetKF((CANVenom_t*)handle, (double)kF);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setB(JNIEnv *, jclass, jlong handle, jdouble b) {
    CANVenom_SetB((CANVenom_t*)handle, (double)b);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setKP(JNIEnv *, jclass, jlong handle, jdouble kP) {
    CANVenom_SetKP((CANVenom_t*)handle, (double)kP);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setKI(JNIEnv *, jclass, jlong handle, jdouble kI) {
    CANVenom_SetKI((CANVenom_t*)handle, (double)kI);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setKD(JNIEnv *, jclass, jlong handle, jdouble kD) {
    CANVenom_SetKD((CANVenom_t*)handle, (double)kD);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setMinPILimit(JNIEnv *, jclass, jlong handle, jdouble limit) {
    CANVenom_SetMinPILimit((CANVenom_t*)handle, (double)limit);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setMaxPILimit(JNIEnv *, jclass, jlong handle, jdouble limit) {
    CANVenom_SetMaxPILimit((CANVenom_t*)handle, (double)limit);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setMaxSpeed(JNIEnv *, jclass, jlong handle, jdouble limit) {
    CANVenom_SetMaxSpeed((CANVenom_t*)handle, (double)limit);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setMaxAcceleration(JNIEnv *, jclass, jlong handle, jdouble limit) {
    CANVenom_SetMaxAcceleration((CANVenom_t*)handle, (double)limit);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setMaxJerk(JNIEnv *, jclass, jlong handle, jdouble limit) {
    CANVenom_SetMaxJerk((CANVenom_t*)handle, (double)limit);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setControlMode(JNIEnv *, jclass, jlong handle, jint controlMode) {
    CANVenom_SetControlMode((CANVenom_t*)handle, (CANVenom_ControlMode)controlMode);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_CANVenomJNI_setBrakeCoastMode(JNIEnv *, jclass, jlong handle, jint brakeCoastMode) {
    CANVenom_SetBrakeCoastMode((CANVenom_t*)handle, (CANVenom_BrakeCoastMode)brakeCoastMode);
}

