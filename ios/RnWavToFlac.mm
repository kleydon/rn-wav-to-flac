#import "RnWavToFlac.h"

#import "../cpp/include/rn-wav-to-flac.hpp"

@implementation RnWavToFlac
RCT_EXPORT_MODULE()



RCT_EXPORT_METHOD(getWavMetaData:(nonnull NSString*)inWavFilePath
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject) {

    NSLog(@"RnWavToFlac.getWavMetaData(%@)\n", inWavFilePath);

    const char *inFilePath = [inWavFilePath cStringUsingEncoding:NSUTF8StringEncoding];
    
    //NSLog(@"  Converted string args:\n");
    //NSLog(@"     inFP: %s\n", inFilePath);
  
    const unsigned int buffSize = 512;
    char buff[buffSize];
    rnwavtoflac::getWavMetaData(inFilePath, buff, buffSize);

    NSString *resultStr = [NSString stringWithUTF8String:buff];

    resolve(resultStr);
}


RCT_EXPORT_METHOD(wavToFlac:(nonnull NSString*)inWavFilePath
                  withOutWavFilePath:(nonnull NSString*)outFlacFilePath
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject) {

    const char *inFilePath = [inWavFilePath cStringUsingEncoding:NSUTF8StringEncoding];
    const char *outFilePath = [outFlacFilePath cStringUsingEncoding:NSUTF8StringEncoding];
    
    NSLog(@"RnWavToFlac.wavToFlac(%@, %@)\n", inWavFilePath, outFlacFilePath);
    // NSLog(@"  Converted string args:\n");
    // NSLog(@"     inFP: %s\n", inFilePath);
    // NSLog(@"     outFP: %s\n", outFilePath);
    
    NSNumber *result = @( rnwavtoflac::wavToFlac(inFilePath, outFilePath) );
    resolve(result);
}


// Example method
// See // https://reactnative.dev/docs/native-modules-ios
RCT_EXPORT_METHOD(multiply:(double)a
                  b:(double)b
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    NSNumber *result = @(rnwavtoflac::multiply(a, b));

    resolve(result);
}



@end
