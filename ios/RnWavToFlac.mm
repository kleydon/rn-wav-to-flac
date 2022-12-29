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
    WavToFlacNameSpace::getWavMetaData(inFilePath, buff, buffSize);

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
    
    NSNumber *result = @( WavToFlacNameSpace::wavToFlac(inFilePath, outFilePath) );
    resolve(result);
}


// Example method for C++
// See the implementation of the example module in the `cpp` folder
RCT_EXPORT_METHOD(multiply:(nonnull NSNumber*)a withB:(nonnull NSNumber*)b
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject) {
    
    NSNumber *result = @(WavToFlacNameSpace::multiply([a floatValue], [b floatValue]));

    resolve(result);
}


@end
