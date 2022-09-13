// WavToFlacEncoder.cpp


#include "WavToFlacEncoder.hpp"


void WavToFlacEncoder::progress_callback(FLAC__uint64 bytes_written,
                                    FLAC__uint64 samples_written,
                                    uint32_t frames_written,
                                    uint32_t total_frames_estimate) {
    fprintf(stderr,
            "wrote %llu bytes, %llu/%u samples, %u/%u frames\n",
            bytes_written,
            samples_written,
            this->totalNumSamples,
            frames_written,
            total_frames_estimate);
}
