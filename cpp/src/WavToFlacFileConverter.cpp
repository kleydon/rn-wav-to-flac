// WavToFlacFileConverter.cpp

#include "WavToFlacFileConverter.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>

#include "FLAC++/metadata.h"
#include "FLAC++/encoder.h"

#include "WavMetadataReader.hpp"
#include "WavToFlacEncoder.hpp"
#include "ResultCode.h"


bool endsWith(const char *str, const char *suffix) {
    if (!str || !suffix) {
        return false;
    }
    size_t strLen = strlen(str);
    size_t suffixLen = strlen(suffix);
    if (suffixLen > strLen) {
        return false;
    }
    return strncmp(str + strLen - suffixLen, suffix, suffixLen) == 0;
}


ResultCode WavToFlacFileConverter::convert(const char* inWavFilePath,
                                       const char* outFlacFilePath) {
    
    printf("wavToFlac()\n");
    printf("  inWavFilePath: %s\n", inWavFilePath);
    printf("  outFlacFilePath: %s\n\n", outFlacFilePath);
    
    // Validate file names
    if (endsWith(inWavFilePath, ".wav") == false) {
        fprintf(stderr, "ERROR: No/invalid suffix for wav file: %s\n", inWavFilePath);
        return RESULT_CODE_ERR_WAV_FILE_SUFFIX;
    }
    if (endsWith(outFlacFilePath, ".flac") == false) {
        fprintf(stderr, "ERROR: No/invalid suffix for flac file: %s\n", outFlacFilePath);
        return RESULT_CODE_ERR_FLAC_FILE_SUFFIX;
    }

    // Read wav file metadata
    WavMetadata wavMetadata;
    WavMetadataReader wavMetadataReader;
    ResultCode rc = wavMetadataReader.readMetadata(inWavFilePath, &wavMetadata);
    if (rc != RESULT_CODE_OK) {
        fprintf(stderr, "ERROR: While reading wav file metadata: %d\n", rc);
        return rc;
    }
    uint32_t sampleRate = wavMetadata.sampleRate;
    uint32_t numChannels = wavMetadata.numChannels;
    uint32_t bitsPerSample = wavMetadata.bitsPerSample;
    uint32_t bytesPerSample = wavMetadata.bitsPerSample / 8;
    uint32_t totalNumSamples = wavMetadata.numSamples; // ui32 fits WAV size limits
    
    // Validate wav file metadata for this application
    if (numChannels < 1 || numChannels > 2) {
        fprintf(stderr,
                "ERROR: Invalid/unsupported number of channels: %d\n",
                numChannels);
        return RESULT_CODE_ERR_NUM_CHANNELS;
    }
    if (bitsPerSample != 8 && bitsPerSample != 16) {
        fprintf(stderr,
                "ERROR: Invalid/unsupported number of bits / sample: %d\n",
                bitsPerSample);
        return RESULT_CODE_ERR_NUM_BITS_PER_SAMPLE;
    }
    
    // Buffers
    const uint32_t wavByteBufferSizeInSamples = 1024;
    const uint32_t wavByteBufferSize =
            wavByteBufferSizeInSamples * bytesPerSample * numChannels;
    const uint32_t pcmBufferSize =
            wavByteBufferSizeInSamples * numChannels;
    FLAC__byte wavByteBuffer[wavByteBufferSize]; // Read WAVE data into here
    FLAC__int32 pcmBuffer[pcmBufferSize];
    
    // FLAC-Specific
    FLAC__StreamEncoderInitStatus initStatus;
    FLAC__StreamMetadata *metadata[2];
    FLAC__StreamMetadata_VorbisComment_Entry entry;
    
    // Open wav file
    FILE *fin;
    if ((fin = fopen(inWavFilePath, "rb")) == NULL) {
        fprintf(stderr, "ERROR: opening %s for reading\n", inWavFilePath);
        fclose(fin);
        return RESULT_CODE_ERR_FILE_OPEN;
    }
    
    // Read wav header bytes (to advance past them)
    if (fread(wavByteBuffer, 1, 44, fin) != 44) {
        fprintf(stderr, "ERROR: Unable to read WAVE header\n");
        fclose(fin);
        return RESULT_CODE_ERR_READ_WAV_HEADER;
    }
    
    // Create encoder
    WavToFlacEncoder encoder(totalNumSamples);
    if (!encoder) {
        fprintf(stderr, "ERROR: allocating encoder\n");
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_ALLOC;
    }
    
    // Configure encoder
    bool ok = encoder.set_verify(true);
    if (!ok) {
        fprintf(stderr, "ERROR: Calling encoder.set_verify()\n");
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_SET_VERIFY;
    }
    const uint32_t compressionLevel = 5;
    ok = encoder.set_compression_level(compressionLevel);
    if (!ok) {
        fprintf(stderr, "ERROR: Calling encoder.set_compression_level(%d)\n", compressionLevel);
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_SET_COMPRESSION_LEVEL;
    }
    ok = encoder.set_channels(numChannels);
    if (!ok) {
        fprintf(stderr, "ERROR: Calling encoder.set_channels(%d)\n", numChannels);
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_SET_CHANNELS;
    }
    ok = encoder.set_bits_per_sample(bitsPerSample);
    if (!ok) {
        fprintf(stderr, "ERROR: Calling encoder.set_bits_per_sample(%d)\n", bitsPerSample);
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_SET_BITS_PER_SAMPLE;
    }
    ok = encoder.set_sample_rate(sampleRate);
    if (!ok) {
        fprintf(stderr, "ERROR: Calling encoder.set_sample_rate(%d)\n", sampleRate);
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_SET_SAMPLE_RATE;
    }
    ok = encoder.set_total_samples_estimate(totalNumSamples);
    if (!ok) {
        fprintf(stderr, "ERROR: Calling encoder.set_total_samples_estimate(%d)\n", totalNumSamples);
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_SET_TOTAL_SAMPLES_ESTIMATE;
    }
    
    // Add som metadata; we'll add tags and a padding block
    if (
        (metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT)) == NULL ||
        (metadata[1] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING)) == NULL ||
        //There are many tag (vorbiscomment) functions but these
        //are convenient for this particular use:
        !FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "ARTIST", "N/A") ||
        !FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false) || /* copy=false: let metadata object take control of entry's allocated string */
        !FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "YEAR", "N/A") ||
        !FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false)
    ) {
        fprintf(stderr, "ERROR: out of memory or tag error\n");
        fclose(fin);
        return RESULT_CODE_ERR_OUT_OF_MEMORY_OR_TAG_ERROR;
    }
    else {
        metadata[1]->length = 1234;  // Set the padding length
        ok = encoder.set_metadata(metadata, 2);
        if (!ok) {
            fprintf(stderr, "ERROR: Calling encoder.set_metadata()\n");
            fclose(fin);
            return RESULT_CODE_ERR_ENCODER_SET_METADATA;
        }
    }
    
    // Initialize the configured encoder / output file
    initStatus = encoder.init(outFlacFilePath);
    if (initStatus != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
        fprintf(stderr, "ERROR: initializing encoder: %s\n", FLAC__StreamEncoderInitStatusString[initStatus]);
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_INIT;
    }

    // Read blocks of samples from WAVE file, feed to encoder
    size_t numSamplesLeft = (size_t)totalNumSamples;
    while(numSamplesLeft) {
        size_t numSamplesToBuffer =
            (numSamplesLeft <= wavByteBufferSizeInSamples) ?
                (size_t)numSamplesLeft : (size_t)wavByteBufferSizeInSamples;
        if (fread(wavByteBuffer, numChannels*bytesPerSample, numSamplesToBuffer, fin) != numSamplesToBuffer) {
            fprintf(stderr, "ERROR: reading from WAVE file\n");
            fclose(fin);
            return RESULT_CODE_ERR_READ_WAV_DATA;
        }
        else {
            // Move the WAV buffer's samples into the FLAC PCM buffer, for libFLAC to handle.
            // The WAV buffer's samples are by default little-endian, packed and interleaved.
            // The FLAC (int32) PCM buffer is interleaved.
            // from WAVE into an interleaved FLAC__int32 buffer for libFLAC
            size_t i;
            for (i = 0; i < numSamplesToBuffer*numChannels; i++) {
                // Approaches below are inefficient but simple, and works on
                // big or little-endian machines
                if (bytesPerSample == 2) {
                    pcmBuffer[i] = (FLAC__int32)
                        ( (
                           (FLAC__int16)(FLAC__int8)wavByteBuffer[2*i+1] << 8
                          ) |
                           (FLAC__int16)wavByteBuffer[2*i]
                        );
                }
                else if (bytesPerSample == 1) {
                    pcmBuffer[i] = (FLAC__int32)(FLAC__int8)(wavByteBuffer[i] - 127);  // 8bit Wav stored as unsigned
                }
                else { // Shouldn't arrive here
                    fprintf(stderr, "ERROR: Bad bytes per sample\n");
                    fclose(fin);
                    return RESULT_CODE_ERR_NUM_BITS_PER_SAMPLE;
                }
            }
            // Feed samples to encoder
            ok = encoder.process_interleaved(pcmBuffer, (uint32_t)numSamplesToBuffer);
            if (!ok) {
                fprintf(stderr, "ERROR: calling encoder.process_interleaved()\n");
                fclose(fin);
                return RESULT_CODE_ERR_ENCODER_PROCESS_INTERLEAVED;
            }
        }
        numSamplesLeft -= numSamplesToBuffer;
    }

    ok = encoder.finish();
    if (!ok) {
        fprintf(stderr, "ERROR: calling encoder.finish()\n");
        fclose(fin);
        return RESULT_CODE_ERR_ENCODER_FINISH;
    }

    printf("Encoding: %s\n", ok ? "SUCCEEDED" : "FAILED");
    printf("   State: %s\n", encoder.get_state().resolved_as_cstring(encoder));

    //Now that encoding is finished, the metadata can be freed
    FLAC__metadata_object_delete(metadata[0]);
    FLAC__metadata_object_delete(metadata[1]);

    fclose(fin);

    return RESULT_CODE_OK;
}
