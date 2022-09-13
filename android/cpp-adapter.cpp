#include <jni.h>
#include <stdio.h>
#include "rn-wav-to-flac.hpp"

extern "C" {

  JNIEXPORT jdouble JNICALL
  Java_com_rnwavtoflac_RnWavToFlacModule_nativeMultiply(JNIEnv *env, jclass type, 
                                                        jdouble a, jdouble b) {

      printf("Java_com_rnwavtoflac_RnWavToFlacModule_nativeMultiplyA()\n");

      return WavToFlacNameSpace::multiply(a, b);
  }


  JNIEXPORT jint JNICALL
  Java_com_rnwavtoflac_RnWavToFlacModule_nativeWavToFlac(JNIEnv *env, jclass type,
                                                        jstring inputWavFilePath,
                                                        jstring outputFlacFilePath) {

      printf("Java_com_rnwavtoflac_RnWavToFlacModule_wavToFlac()\n");

      // Convert from jstring -> const char*
      const char *inWavFilePathStr = env->GetStringUTFChars(inputWavFilePath, 0);
      const char *outFlacFilePathStr = env->GetStringUTFChars(outputFlacFilePath, 0);
      printf("  inWavFilePathStr: %s\n", inWavFilePathStr);
      printf("  outFlacFilePathStr: %s\n", outFlacFilePathStr);

      return WavToFlacNameSpace::wavToFlac(inWavFilePathStr, outFlacFilePathStr);
  }

}


