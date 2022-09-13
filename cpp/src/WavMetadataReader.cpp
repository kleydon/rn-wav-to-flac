// ReadWavMetadata.cpp

#include <string.h>

#include "WavMetadataReader.hpp"

// NOTE: This code *MAY* be dependent on little-endian architecture.
// (Is that a problem?)
// flac/examples/cpp/encode offers an alternative approach
// which may be endedness-independent....


ResultCode WavMetadataReader::readMetadata(const char* wavFilePath,
                                           WavMetadata* wavMetadata) {
    
    // Open file
    FILE* fp = fopen(wavFilePath, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "ERROR: Input file not found: %s\n", wavFilePath);
        fclose(fp);
        return RESULT_CODE_ERR_FILE_OPEN;
    }
    
    // Read file's metadata
    // +++++++++++++
    const int STRCMP_OK = 0;
    char id[5];
    uint32_t size;
    uint16_t formatTag, numChannels, blockAlign, bitsPerSample;
    uint32_t formatLength, sampleRate, avgBytesPerSec, dataSize;
    
    fread(id, sizeof(uint8_t), 4, fp);
    id[4] = '\0';
    
    if (strcmp(id, "RIFF") != STRCMP_OK) {
        fprintf(stderr, "ERROR: not a RIFF file.\n");
        fclose(fp);
        return RESULT_CODE_ERR_NO_RIFF;
    }
        
    fread(&size, sizeof(uint32_t), 1, fp);
    //printf("Size: %u\n", size);
    
    fread(id, sizeof(uint8_t), 4, fp);
    id[4] = '\0';
    
    if (strcmp(id, "WAVE") != STRCMP_OK) {
        fprintf(stderr, "Error: RIFF file, but not a WAVE file\n");
        fclose(fp);
        return RESULT_CODE_ERR_NO_WAVE;
    }
        
    fread(id, sizeof(uint8_t), 4, fp);
    
    fread(&formatLength, sizeof(unsigned int), 1, fp);
    
    fread(&formatTag, sizeof(int16_t), 1, fp);
    
    fread(&numChannels, 2, 1, fp);
    //printf("numChannels: %u\n", numChannels);
    
    fread(&sampleRate, 4, 1, fp);
    //printf("Sample rate: %u\n", sampleRate);
    
    fread(&avgBytesPerSec, 4, 1, fp);
    //printf("Avg bytes/sec: %u\n", avgBytesPerSec);
    
    fread(&blockAlign, 2, 1, fp);
    //printf("Block Align: %u\n", blockAlign);
    
    fread(&bitsPerSample, 2, 1, fp);
    //printf("Bits/Sample: %u\n", bitsPerSample);
    
    fread(id, 1, 4, fp);
    
    fread(&dataSize, 4, 1, fp);
    //printf("Data Size: %u\n", dataSize);
    
    uint32_t bytesPerSample = bitsPerSample / 8;
    uint32_t numSamples = dataSize / (bytesPerSample * numChannels);
    
    //Verify number of channels
    if (numChannels < 1 || numChannels > 2) {
        fprintf(stderr, "ERROR: Bad number of channels: %d\n", numChannels);
        fclose(fp);
        return RESULT_CODE_ERR_NUM_CHANNELS;
    }
    
    //Verify bit depth
    if (bitsPerSample != 8 && bitsPerSample != 16) {
        fprintf(stderr, "ERROR: Bad bitsPerSample: %d\n", bitsPerSample);
        fclose(fp);
        return RESULT_CODE_ERR_NUM_BITS_PER_SAMPLE;
    }

    fclose(fp);
    // -------------

    // Compose result
    wavMetadata->sampleRate = sampleRate;
    wavMetadata->numChannels = numChannels;
    wavMetadata->bitsPerSample = bitsPerSample;
    wavMetadata->numSamples = numSamples;
    
    return RESULT_CODE_OK;
}

