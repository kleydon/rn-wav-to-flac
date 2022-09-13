// ResultCode.h

#ifndef __RESULT_CODE_H__
#define __RESULT_CODE_H__

enum ResultCode {
    //Ok code
    RESULT_CODE_OK = 0,
    //Error codes
    RESULT_CODE_ERR_UNSPECIFIED = 1,
    RESULT_CODE_ERR_ARGS = 2,
    RESULT_CODE_ERR_WAV_FILE_SUFFIX = 3,
    RESULT_CODE_ERR_FLAC_FILE_SUFFIX = 4,
    RESULT_CODE_ERR_FILE_OPEN = 5,
    RESULT_CODE_ERR_READ_WAV_HEADER = 6,
    RESULT_CODE_ERR_READ_WAV_DATA = 7,
    RESULT_CODE_ERR_INVALID_OR_UNSUPPORTED_WAV_FILE = 8,
    RESULT_CODE_ERR_ENCODER_ALLOC = 9,
    RESULT_CODE_ERR_ENCODER_SET_VERIFY = 10,
    RESULT_CODE_ERR_ENCODER_SET_COMPRESSION_LEVEL = 11,
    RESULT_CODE_ERR_ENCODER_SET_CHANNELS = 12,
    RESULT_CODE_ERR_ENCODER_SET_BITS_PER_SAMPLE = 13,
    RESULT_CODE_ERR_ENCODER_SET_SAMPLE_RATE = 14,
    RESULT_CODE_ERR_ENCODER_SET_TOTAL_SAMPLES_ESTIMATE = 15,
    RESULT_CODE_ERR_ENCODER_PROCESS_INTERLEAVED = 16,
    RESULT_CODE_ERR_ENCODER_FINISH = 17,
    RESULT_CODE_ERR_OUT_OF_MEMORY_OR_TAG_ERROR = 18,
    RESULT_CODE_ERR_ENCODER_SET_METADATA = 19,
    RESULT_CODE_ERR_ENCODER_INIT = 20,
    RESULT_CODE_ERR_NO_RIFF = 21,
    RESULT_CODE_ERR_NO_WAVE = 22,
    RESULT_CODE_ERR_NUM_CHANNELS = 23,
    RESULT_CODE_ERR_NUM_BITS_PER_SAMPLE = 24,
};

#endif  // __RESULT_CODE_H__