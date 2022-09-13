#import "RnWavToFlac.h"

#import "rn-wav-to-flac.h"
#import "WavToFlacFileConverter.hpp"

@implementation RnWavToFlac
RCT_EXPORT_MODULE()


// Example method for C++
// See the implementation of the example module in the `cpp` folder
RCT_EXPORT_METHOD(multiply:(nonnull NSNumber*)a withB:(nonnull NSNumber*)b
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject) {
    
    NSNumber *result = @(example::multiply([a floatValue], [b floatValue]));

    resolve(result);
}


RCT_EXPORT_METHOD(wavToFlac:(nonnull NSString*)inWavFilePath
                  withOutWavFilePath:(nonnull NSString*)outFlacFilePath
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject) {

    const char *inFilePath = [inWavFilePath cStringUsingEncoding:NSUTF8StringEncoding];
    const char *outFilePath = [outFlacFilePath cStringUsingEncoding:NSUTF8StringEncoding];
    
    NSLog(@"RnWavToFlac.wavToFlac(%@, %@)\n", inWavFilePath, outFlacFilePath);
    NSLog(@"  Converted string args:\n");
    NSLog(@"     inFP: %s\n", inFilePath);
    NSLog(@"     outFP: %s\n", outFilePath);
    
    NSNumber *result = @( WavToFlacFileConverter::convert(inFilePath, outFilePath));
    resolve(result);
}


@end
