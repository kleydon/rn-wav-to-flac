#include <jni.h>
#include "rn-wav-to-flac.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_rnwavtoflac_RnWavToFlacModule_nativeMultiply(JNIEnv *env, jclass type, jint a, jint b) {
    return example::multiply(a, b);
}
