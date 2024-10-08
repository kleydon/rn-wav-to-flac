# rn-wav-to-flac
Converts wav files to flac files


## Compatibility:

React Native >= 0.61
iOS: >= 11.0
Android SDK: >= 21


## Installation

In your project directory, type:
```
yarn add 'rn-wav-to-flac@https://github.com/kleydon/rn-wav-to-flac'
```
[iOS only]:
```
npx pod-install
```

## Post-Installation

[Android only]:

Add the following permissions to your application's AndroidManifest.xml:
```xml
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

## Usage

```ts
import { wavToFlac } from "rn-wav-to-flac"
// ...
const resultCode = await wavToFlac(inputWavFilePath, outputFlacFilePath)
// Result codes listed in cpp/include/ResultCode.h; Success == 0
```

## Limitations

There are some directories this module simply won't have read/write access to. It is recommended to:

1. Add a cross-platform file system management utility to your project, e.g: 
```
yarn add 'react-native-fs'
```

2. Use directories supplied by this file system manager, e.g:
```
import RNFS from 'react-native-fs'
const cacheDirPath = RNFS.CachesDirectoryPath
const inputWavFilePath = cacheDirPath + '/inputWavFile.wav'
const outputFlacFilePath = cacheDirPath + '/outputFlacFile.flac'
```

While `RNFS.CacheDirectoryPath` has been tested, other paths have not; experimentation required.


## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.


## License & Attribution

See LICENSE file.

**rn-wav-to-flac:**
  Copyright (C) 2022-Present Krispin Leydon, All rights reserved

**libFLAC/libFLAC++:**
  Copyright (C) 2000-2009  Josh Coalson
  Copyright (C) 2011-2022  Xiph.Org Foundation


## Development Process

Developing react-native modules is slow going; it is typically necessary to work in (at least) 3 languages simultaneously, and easy to make mistakes. Take your time, be deliberate, save your work through frequent small commits. 

**When project settings get messed up, it is often easier to build a new project from scratch using [create-react-native-library](https://github.com/callstack/react-native-builder-bob) - see below - then re-import your functional code into this new, up-to-date project skeleton.**

**Don't mindlessly update project settings, when XCode and Android Studio suggest to do this! Where possible, stick with the defaults provided by [create-react-native-library](https://github.com/callstack/react-native-builder-bob).**

**Don't cavalierly upgrade react-native; preview with (react-native upgrade helper)[https://react-native-community.github.io/upgrade-helper/]. Probably easier to rebuild the project with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)!**

**You may need to run a custom ruby install, and (if you're running iOS) you won't want to mess with the default installation! I recommend chruby, with ruby-install.**

**If installing npx pod-install / pod install for iOS is giving you problems, you *MAY* have hit this bug:**
https://github.com/facebook/react-native/issues/39832
Try running  bundle update --bundler in the project root directory (which may be example/ if building the example), then `bundle install` and `bundle exec pod install` in the ios directory

**If you get a "PhaseScriptExecution" bug - make sure that the directory path to your project doesn't include any spaces (doh!)**

***Async arrow functions are currently (Aug 16, 2024) unsupported by the hermes js engine.* using them can cause unpredictable effects. Instead (until there IS support) use async non-arrow functions, but provide access to the outer "this" if need be**

**To deal with Yarn idiocy, see: https://levelup.gitconnected.com/how-to-use-yarn-3-with-react-native-and-how-to-migrate-c5f108108533.**


### 1. Get FLAC distribution

Download flac-1.3.4 source distribution from:
	https://ftp.osuosl.org/pub/xiph/releases/flac/


### 2. Build libFLAC/libFLAC++ according to distribution's README instructions for GNU toolchain
I.e: `./configure && make && make check && make install`


### 3. Create XCode C++ command line test app linked against libFLAC/libFLAC++ libs, using the following project settings:

**XCode Project Settings:**
Project > Target > Build Settings:
    * Library Search Paths: Add /usr/local/lib  (non-recursive)
    * Header Search Paths: Add /usr/local/include (non-recursive)

Project > Target > Build Phases:
    * Link Binary with Libraries (or their symlinks): libFLAC.a, libFLAC++.a
      (And/or drag these libs to the "Frameworks" group)
    * Copy Files Absolute Path: ${SRCROOT}/Run
      Drag in product binary so it exists at a stable, known location.

Project > Target > Signing & Capabilities > Hardened Runtime:
    * Disable Libary Validation


### 4. Recreate XCode C++ command line test app, now compiling in libFLAC/libFLAC++ from source, using the following project settings and simplifications:

**XCode Project Settings:**
Project > Target > Build Phases:
   * Copy Files > Absolute Path: ${SRCROOT}/Run
     Drag in product binary, so it exists at a stable, known location.

   * In Project > Target > Build Settings > Search Paths > Header Search Paths, add:
      ${SRCROOT}/OSXWavToFlacV2/Lib/include (recursively)
      ${SRCROOT}/OSXWavToFlacV2/Lib/libFLAC/include (non-recursive)


**FLAC source code simplifications:**

Recursively copy flac source to project's "cpp" folder:
   <flac-scr-distribution>/include/ -> cpp/include dir
   <flac-scr-distribution>/src/libFLAC -> cpp/libFLAC
   <flac-scr-distribution>/src/libFLAC++ -> cpp/libFLAC++
   <flac-scr-distribution>/src/share -> cpp/share

Recursively remove all non-header, non-source build/config files from:
   cpp/include
   cpp/libFLAC
   cpp/libFLAC++
   cpp/share

Recursively remove optional GPL-licensed files:
   Remove cpp/share/getopt/
   Remove cpp/share/utf8/
   Remove cpp/include/share/getopt.h
   Remove cpp/include/share/utf8.h

Recursively remove optional assembly code optimizations:
   Remove cpp/libFLAC/as32/

Recursively remove windows-specific files:
   Remove cpp/libFLAC/windows_unicode_filenames.c
   Remove cpp/include/share/win_utf8_io/win_utf8_io.h
   Removed cpp/share/win_utf8_io/

   Remove ogg-specific files:
     Remove cpp/libFLAC/include/private/ogg_encoder_aspect.h
     Remove cpp/libFLAC/include/private/ogg_decoder_aspect.h
     Remove cpp/libFLAC/include/private/ogg_helper.h
     Remove cpp/libFLAC/ogg_encoder_aspect.c
     Remove cpp/libFLAC/ogg_decoder_aspect.c
     Remove cpp/libFLAC/ogg_helper.c

In cpp/libFLAC/stream_encoder.c and stream_decoder.c, disable OGG, via the addition:
```
   // Disable OGG
   #define FLAC__HAS_OGG 0
```

In cpp/include/FLAC/assert.h, make the following modification:
```
    // MODIFICATION:
    // To get rid of XCode's "Implicit declaration of function
    // 'assert' is invalid in C99" error, commented out:
    //       #define FLAC__ASSERT(x) assert(x)
    // ...and added:
    #define FLAC__ASSERT(x)
```

In cpp/libFLAC/lpc.c, make the following modification:
```
    // MODIFICATION:
    // b/c lround(double) already defined in /usr/include/math.h
    #define HAVE_LROUND 1
```
In cpp/libFLAC/format.c, make the modification:
```
    // MODIFICATION - Add:
    #define PACKAGE_VERSION "1.3.4"
```
In cpp/share/grabbag/cuesheet.c, added the following modification:
    Change all occurrances of `PRIu64` to `llu`



### 5. Fold libFLAC/libFLAC++ into a React-Native Library

Create the skeleton rn lib via `npx create-react-native-library rn-wav-to-flac`, choosing Native Module > C++ for Android & iOS > Vanilla. Test on iOS/android. (There's an example of working with Kotlin here: // See: https://medium.com/@mregnauld/how-to-run-c-code-from-a-native-android-or-ios-app-265cad775ed5).

Change ios bundle identifier to `com.rnwavtoflac`

Place libFLAC/libFLAC++ src and includes (and my wavToFlac conversion code) in the "cpp" dir that was created with the RN lib.

Rename cpp/include/rn-wav-to-flac.h -> cpp/include/rn-wav-to-flac.hpp

Configure module's ios XCode project (ios/RnWavToFlac.xcodeproj) as follows:
	
	Header Search Paths:
	$(SRCROOT)/../cpp  (recursive)

Rename flac's "float.h" to "flacFloat.h", to prevent a naming collision on ios (which resulted in ios library files being unable to find `FLT_MIN` / `FLT_MAX` constants.

Modify rn-wav-to-flac.podspec to include:
```
  # Modified section
  # ++++++++++++++++
  # Modified to admit 'c' and 'hpp' files
  s.source_files = "ios/**/*.{h,m,mm}", "cpp/**/*.{h,c,hpp,cpp}"

  # Added to specify header search paths for BUILDING the pod
  # Idea from here: https://github.com/CocoaPods/CocoaPods/issues/5375
  s.pod_target_xcconfig = {
    'HEADER_SEARCH_PATHS' => '$(inherited) ${PODS_TARGET_SRCROOT}/ios/** ${PODS_TARGET_SRCROOT}/cpp/**'
  }

  # Added to expose a heirarchical header structure for the USERS of this pod.
  # Other methods, e.g. s.private_header_files, s.public_header_files etc, 
  # flatten the heirarchy, resulting in naming collisions and confusion.
  s.header_mappings_dir = 'cpp/include'
  # ----------------
```

Modify example/ios/Podfile to include:
```
  # Attempt to "sandbox" this pod, header-wise, from other pods.
  # NOTE: Doesn't isolate from some other application code (which is why 
  # we needed to rename flac's "float.h" to "flac_float.h"...)
  pod 'rn-wav-to-flac', path: '../..', modular_headers: true
```

Modify android/CMakeLists.txt to incorporate:
	* all cpp/src and cpp/include files
	* cpp-adapter.cpp
  NOTE: It seems like the functions in this file don't allow for recursivity (?)

Modify android/src/main/java/com/rnwavtoflac/RnWavToFlacModule.java, to define:
	* The native module name (keep this in sync with name in CMakLists.txt!)
	* The native method signatures (keep these in sync with cpp-adapter.cpp!)
	* The ReactNative methods in Java

Update the following, to keep in sync:
	iOS:
	/ios/RnWavToFlac.mm
	Android:
	/android/src/main/java/com/rnwavtoflac/RnWavToFlacModule.java
	android/cpp-adapter.cpp
	C++:
	android/CMakeLists.txt
	cpp/include/rn-wav-to-flac.hpp
	cpp/source/rn-wav-to-flac.cpp
	cpp/include/WavToFlacFileConverter.hpp
	cpp/source/WavToFlacFileConverter.cpp


