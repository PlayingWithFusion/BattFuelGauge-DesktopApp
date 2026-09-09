#include "jni.h"
#include "com_playingwithfusion_jni_TimeOfFlightJNI.h"
#include "TimeOfFlightApi.h"

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_create(JNIEnv *, jclass, jint sensorId) {
    TimeOfFlight_t *pHandle;

    pHandle = TimeOfFlight_Create((uint8_t)sensorId);
    return (jlong)pHandle;
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_destroy(JNIEnv *, jclass, jlong handle) {
    TimeOfFlight_Destroy((TimeOfFlight_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_identifySensor(JNIEnv *, jclass, jlong handle) {
    TimeOfFlight_IdentifySensor((TimeOfFlight_t*)handle);
}

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getFirmwareVersion(JNIEnv *, jclass, jlong handle) {
    return (jlong)TimeOfFlight_GetFirmwareVersion((TimeOfFlight_t*)handle);
}

JNIEXPORT jlong JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getSerialNumber(JNIEnv *, jclass, jlong handle) {
    return (jlong)TimeOfFlight_GetSerialNumber((TimeOfFlight_t*)handle);
}

JNIEXPORT jboolean JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_isRangeValid(JNIEnv *, jclass, jlong handle) {
    return (jboolean)TimeOfFlight_IsRangeValid((TimeOfFlight_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getRange(JNIEnv *, jclass, jlong handle) {
    return (jdouble)TimeOfFlight_GetRange((TimeOfFlight_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getRangeSigma(JNIEnv *, jclass, jlong handle) {
    return (jdouble)TimeOfFlight_GetRangeSigma((TimeOfFlight_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getAmbientLightLevel(JNIEnv *, jclass, jlong handle) {
    return (jdouble)TimeOfFlight_GetAmbientLightLevel((TimeOfFlight_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getStatus(JNIEnv *, jclass, jlong handle) {
    return (jint)TimeOfFlight_GetStatus((TimeOfFlight_t*)handle);
}

JNIEXPORT jint JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getRangingMode(JNIEnv *, jclass, jlong handle) {
    return (jint)TimeOfFlight_GetRangingMode((TimeOfFlight_t*)handle);
}

JNIEXPORT jdouble JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_getSampleTime(JNIEnv *, jclass, jlong handle) {
    return (jdouble)TimeOfFlight_GetSampleTime((TimeOfFlight_t*)handle);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_setRangingMode(JNIEnv *, jclass, jlong handle, jint mode, jdouble sampleTime) {
    TimeOfFlight_SetRangingMode((TimeOfFlight_t*)handle, (TimeOfFlight_RangingMode)mode, (double)sampleTime);
}

JNIEXPORT void JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_setRangeOfInterest(JNIEnv *, jclass, jlong handle, jint topLeftX, jint topLeftY, jint bottomRightX, jint bottomRightY) {
    TimeOfFlight_SetRangeOfInterest((TimeOfFlight_t*)handle, (uint8_t)topLeftX, (uint8_t)topLeftY, (uint8_t)bottomRightX, (uint8_t)bottomRightY);
}

JNIEXPORT jboolean JNICALL Java_com_playingwithfusion_jni_TimeOfFlightJNI_isConnected(JNIEnv *, jclass, jlong handle) {
    return (jboolean)TimeOfFlight_IsConnected((TimeOfFlight_t*)handle);
}