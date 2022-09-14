// ReadWavMetadata.cpp

#include <string.h>

#include "WavMetadataReader.hpp"

// NOTE: This code *MAY* be dependent on little-endian architecture.
// (Is that a problem?)
// flac/examples/cpp/encode offers an alternative approach
// which may be endedness-independent....


void safeCloseFile(FILE* fp) {
  if (fp) {
    fclose(fp);
  }
}


ResultCode WavMetadataReader::readMetadata(const char* wavFilePath,
                                           WavMetadata* wavMetadata) {
    
    // Open file
    FILE* fp = fopen(wavFilePath, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "ERROR: Input file not found: %s\n", wavFilePath);
        return RESULT_CODE_ERR_FILE_OPEN;
    }
    
    // Read file's metadata
    // +++++++++++++
    const int STRCMP_OK = 0;
    char id[5];
    uint32_t size;
    uint16_t formatTag, numChannels, blockAlign, bitsPerSample;
    uint32_t formatLength, sampleRate, avgBytesPerSec, dataSize;
    
    // RIFF chunk
    
    if (fread(id, 1, 4, fp) != 4) {
        fprintf(stderr, "ERROR: Reading RIFF chunk header; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_RIFF_CHUNK_HEADER;
    }
    id[4] = '\0';
    
    if (strcmp(id, "RIFF") != STRCMP_OK) {
        fprintf(stderr, "ERROR: no RIFF chunk header.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_NO_RIFF_CHUNK_HEADER;
    }
    
    if (fread(&size, 4, 1, fp) != 1){
        fprintf(stderr, "ERROR: Reading RIFF chunk size; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_RIFF_CHUNK_SIZE;
    }
    //printf("Size: %u\n", size);
    
    if (fread(id, 1, 4, fp) != 4) {
        fprintf(stderr, "ERROR: Reading WAVE; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_RIFF_CHUNK_WAVE;
    }
    id[4] = '\0';
    
    if (strcmp(id, "WAVE") != STRCMP_OK) {
        fprintf(stderr, "Error: RIFF chunk does not contain WAVE\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_NO_RIFF_CHUNK_WAVE;
    }
    
    // 'fmt ' or 'JUNK' chunk
    
    if (fread(id, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Reading post-RIFF chunk id, not enough elements read\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_POST_RIFF_CHUNK_HEADER;
    }
    id[4] = '\0';
  
    // If its JUNK...
    // See: https://www.daubnet.com/en/file-format-riff
    if (strcmp(id, "JUNK") == STRCMP_OK) {
        // Get size
        if (fread(&size, 4, 1, fp) != 1) {
            fprintf(stderr, "Error: Reading JUNK chunk size, not enough elements read\n");
            safeCloseFile(fp);
            return RESULT_CODE_ERR_READ_JUNK_CHUNK_SIZE;
        }
        //printf("Junk size: %u\n", size);
        // Tack on extra byte if size is odd
        // See: https://www.daubnet.com/en/file-format-riff
        size += (size % 2);
        //printf("Adjusted junk size: %u\n", size);
        // Advance past the rest of the bytes in this chunk
        char c;
        for (int i = 0; i < size; i++) {
            if (fread(&c, 1, 1, fp) != 1) {
                fprintf(stderr, "Error: Reading JUNK chunk data, not enough elements read\n");
                safeCloseFile(fp);
                return RESULT_CODE_ERR_READ_JUNK_CHUNK_DATA;
            }
        }
        //Get the NEXT chunk id - should be "fmt "
        if (fread(id, 1, 4, fp) != 4) {
            fprintf(stderr, "Error: Reading post-JUNK chunk id, not enough elements read\n");
            safeCloseFile(fp);
            return RESULT_CODE_ERR_READ_FMT_CHUNK_HEADER;
        }
        id[4] = '\0';
    }
    
    if (strcmp(id, "fmt ") != STRCMP_OK) {
        fprintf(stderr, "Error: No 'fmt ' chunk header\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_NO_FMT_CHUNK_HEADER;
    }
    
    if (fread(&formatLength, 4, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk length; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_SIZE;
    }
    
    if (fread(&formatTag, 2, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk tag; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_TAG;
    }
    
    if (fread(&numChannels, 2, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk numChannels; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_NUM_CHANNELS;
    }
    //printf("numChannels: %u\n", numChannels);
    
    if (fread(&sampleRate, 4, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk sampleRate; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_SAMPLE_RATE;
    }
    //printf("Sample rate: %u\n", sampleRate);
    
    if (fread(&avgBytesPerSec, 4, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk avgBytesPerSec; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_AVG_BYTES_PER_SEC;
    }
    //printf("Avg bytes/sec: %u\n", avgBytesPerSec);
    
    if (fread(&blockAlign, 2, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk blockAlign; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_BLOCK_ALIGN;
    }
    //printf("Block Align: %u\n", blockAlign);
    
    if (fread(&bitsPerSample, 2, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'fmt ' chunk bitsPerSample; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_FMT_CHUNK_BITS_PER_SAMPLE;
    }
    //printf("Bits/Sample: %u\n", bitsPerSample);
    
    // Post-fmt chunk
        
    if (fread(id, 1, 4, fp) != 4) {
        fprintf(stderr, "ERROR: Reading post-'fmt' chunk header; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_POST_FMT_CHUNK_HEADER;
    }
    id[4] = '\0';
    
    // If its FLLR
    // Apple adds a "FLLR" chunk between 'fmt ' and 'data'
    // https://stackoverflow.com/a/34278044
    // https://stackoverflow.com/questions/6284651/avaudiorecorder-doesnt-write-out-proper-wav-file-header
    //https://github.com/swharden/Spectrogram/issues/12
    if (strcmp(id, "FLLR") == STRCMP_OK) {
        // Get size
        if (fread(&size, 4, 1, fp) != 1) {
            fprintf(stderr, "Error: Reading FLLR chunk size, not enough elements read\n");
            safeCloseFile(fp);
            return RESULT_CODE_ERR_READ_FLLR_CHUNK_SIZE;
        }
        printf("FLLR chunk size: %u\n", size);
        // Advance past the rest of the bytes in this chunk
        //printf("Adjusted junk size: %u\n", size);
        char filler[size];
        if (fread(filler, 1, size, fp) != size) {
            fprintf(stderr, "Error: Reading FLLR chunk data, not enough elements read\n");
            safeCloseFile(fp);
            return RESULT_CODE_ERR_READ_FLLR_CHUNK_DATA;
        }
        
        //Get to the NEXT chunk id - should be "data"
        if (fread(id, 1, 4, fp) != 4) {
            fprintf(stderr, "Error: Reading post-FLLR chunk id, not enough elements read\n");
            safeCloseFile(fp);
            return RESULT_CODE_ERR_READ_DATA_CHUNK_HEADER;
        }
        id[4] = '\0';
    }
    
    if (strcmp(id, "data") != STRCMP_OK) {
        fprintf(stderr, "Error: No 'data' chunk header: %s\n", id);
        safeCloseFile(fp);
        return RESULT_CODE_ERR_NO_DATA_CHUNK_HEADER;
    }
    
    if (fread(&dataSize, 4, 1, fp) != 1) {
        fprintf(stderr, "ERROR: Reading 'data' chunk size; not enough elements read.\n");
        safeCloseFile(fp);
        return RESULT_CODE_ERR_READ_DATA_CHUNK_SIZE;
    }
    //printf("Data Size: %u\n", dataSize);
    
    uint32_t bytesPerSample = bitsPerSample / 8;
    uint32_t numSamples = dataSize / (bytesPerSample * numChannels);
    
    uint64_t numBytesBeforeSampleData = ftell(fp);
    printf("numBytesBeforeSampleData: %lu\n", numBytesBeforeSampleData);
    
    //Verify number of channels
    if (numChannels < 1 || numChannels > 2) {
        fprintf(stderr, "ERROR: Bad number of channels: %d\n", numChannels);
        safeCloseFile(fp);
        return RESULT_CODE_ERR_INVALID_NUM_CHANNELS;
    }
    
    //Verify bit depth
    if (bitsPerSample != 8 && bitsPerSample != 16) {
        fprintf(stderr, "ERROR: Bad bitsPerSample: %d\n", bitsPerSample);
        safeCloseFile(fp);
        return RESULT_CODE_ERR_INVALID_NUM_BITS_PER_SAMPLE;
    }
        
    safeCloseFile(fp);
    // -------------

    // Compose result
    wavMetadata->sampleRate = sampleRate;
    wavMetadata->numChannels = numChannels;
    wavMetadata->bitsPerSample = bitsPerSample;
    wavMetadata->numSamples = numSamples;
    wavMetadata->numBytesBeforeSampleData = numBytesBeforeSampleData;
    
    return RESULT_CODE_OK;
}

