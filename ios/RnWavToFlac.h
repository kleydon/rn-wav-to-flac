#ifdef __cplusplus
    //#import "rn-wav-to-flac.hpp"
    #import "../cpp/include/rn-wav-to-flac.hpp"
#endif

//#ifdef RCT_NEW_ARCH_ENABLED
//    #import "RNRnWavToFlacSpec.h"
//    @interface RnWavToFlac : NSObject <NativeRnWavToFlacSpec>
//#else
    #import <React/RCTBridgeModule.h>
    @interface RnWavToFlac : NSObject <RCTBridgeModule>
//#endif

@end
