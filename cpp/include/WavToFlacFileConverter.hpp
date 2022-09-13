// WavToFlacFileConverter.hpp

#ifndef __WAV_TO_FLAC_FILE_CONVERTER_HPP__
#define __WAV_TO_FLAC_FILE_CONVERTER_HPP__

#include "ResultCode.h"


class WavToFlacFileConverter {
    public:
        static ResultCode convert(const char* inWavFilePath, const char* outFlacFilePath);
};


#endif //__WAV_TO_FLAC_FILE_CONVERTER_HPP__
