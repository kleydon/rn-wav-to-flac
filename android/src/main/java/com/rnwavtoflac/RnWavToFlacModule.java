package com.rnwavtoflac;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

import android.util.Log;

@ReactModule(name = RnWavToFlacModule.NAME)
public class RnWavToFlacModule extends ReactContextBaseJavaModule {
  
    public static final String NAME = "RnWavToFlac";

    public RnWavToFlacModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    @NonNull
    public String getName() {
        return NAME;
    }

    static {
        final String nativeLibName = "WavToFlac";  // Defined in android/CMakeList.txt; keep in sync
        try {
            // Load 'native-lib' library on application startup.
            System.loadLibrary(nativeLibName);
            Log.e(NAME, "Loaded native library: " + nativeLibName);
        } 
        catch (Exception e) {
            Log.e(NAME, "Unable to load native library: " + nativeLibName);
        }
    }

    // Native method signatures underpinning react methods; see cpp-adapter.cpp for definitions

    public static native double nativeMultiply(double a, double b);
    public static native int nativeWavToFlac(String inputWavFilePath, String outputFlacFilePath);
    
    // React methods

    @ReactMethod
    public void multiply(double a, double b, Promise promise) {
        promise.resolve(nativeMultiply(a, b));
    }

    @ReactMethod
    public void wavToFlac(String inputWavFilePath, String outputFlacFilePath, Promise promise) {
        promise.resolve(nativeWavToFlac(inputWavFilePath, outputFlacFilePath));
    }
}
