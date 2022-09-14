//  ReadWavMetadata.hpp

#ifndef READ_WAV_METADATA_HPP
#define READ_WAV_METADATA_HPP

#include <stdio.h>
#include <stdint.h>
#include "ResultCode.h"

void safeCloseFile(FILE* fp);

struct WavMetadata {
    uint16_t bitsPerSample;  // For a single channel
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t numSamples;  // 32-bits ok, due to WAVE size limitations
    uint64_t numBytesBeforeSampleData;  // For header, etc
};

class WavMetadataReader {
    public:
        ResultCode readMetadata(const char* wavFilePath,
                                WavMetadata* wavMetadata);
};

#endif  // READ_WAV_METADATA_HPP
