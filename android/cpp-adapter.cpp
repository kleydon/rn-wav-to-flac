#include <jni.h>
#include "rn-wav-to-flac.h"

extern "C"
JNIEXPORT jint JNICALL

// Java_com_rnwavtoflac_RnWavToFlacModule_nativeMultiply(JNIEnv *env, jclass type, jint a, jint b) {
//     return example::multiply(a, b);
// }

//Changed "jint" to "jdouble" below

Java_com_rnwavtoflac_RnWavToFlacModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return example::multiply(a, b);
}
