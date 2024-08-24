package com.rnwavtoflac

import android.util.Log

import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.bridge.Promise

// For notes on calling C++ from Kotlin, see: 
// https://medium.com/@mregnauld/how-to-run-c-code-from-a-native-android-or-ios-app-265cad775ed5

class RnWavToFlacModule(reactContext: ReactApplicationContext) : ReactContextBaseJavaModule(reactContext) {

  companion object {
    const val NAME = "RnWavToFlac"
    init {
      System.loadLibrary("rn-wav-to-flac")
    }
    // NDK functions, here declared as static:
    @JvmStatic private external fun nativeMultiply(a:Double, b:Double): Double
    @JvmStatic private external fun nativeWavToFlac(inputWavFilePath:String, outputFlacFilePath:String): Int
    @JvmStatic private external fun nativeGetWavMetaData(inputWavFilePath:String): String
  }

  override fun getName(): String {
    return NAME
  }



  // React methods

  // Example method
  // See https://reactnative.dev/docs/native-modules-android
  @ReactMethod fun multiply(a: Double, b: Double, promise: Promise) {
    promise.resolve(nativeMultiply(a, b))
  }

  @ReactMethod fun getWavMetaData(inputWavFilePath:String, promise:Promise) {
    promise.resolve(nativeGetWavMetaData(inputWavFilePath))
  }

  @ReactMethod fun wavToFlac(inputWavFilePath:String, outputFlacFilePath:String, promise:Promise) {
    promise.resolve(nativeWavToFlac(inputWavFilePath, outputFlacFilePath))
  }
}
