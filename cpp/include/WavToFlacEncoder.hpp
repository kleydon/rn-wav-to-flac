// WavToFlacEncoder.hpp

// Subclass of FLAC::Encoder::File, with an implementation for progress_callback()


#ifndef __WAV_TO_FLAC_ENCODER_HPP__
#define __WAV_TO_FLAC_ENCODER_HPP__

#include <stdio.h>
#include <stdint.h>

#include "FLAC++/metadata.h"
#include "FLAC++/encoder.h"
#include "FLAC/stream_encoder.h"

class WavToFlacEncoder: public FLAC::Encoder::File {
    public:
    WavToFlacEncoder(uint32_t totalNumSamples): FLAC::Encoder::File() {
            this->totalNumSamples = totalNumSamples;
        }
    protected:
        uint32_t totalNumSamples = 0; // 32-bits ok due to WAVE size limitations
        virtual void progress_callback(FLAC__uint64 bytes_written,
                                       FLAC__uint64 samples_written,
                                       uint32_t frames_written,
                                       uint32_t total_frames_estimate);
};

#endif //__WAV_TO_FLAC_ENCODER_HPP__
