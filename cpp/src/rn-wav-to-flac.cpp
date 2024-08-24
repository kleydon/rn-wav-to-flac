#include "rn-wav-to-flac.hpp"
#include "WavMetadataReader.hpp"
#include "WavToFlacFileConverter.hpp"

namespace rnwavtoflac {

  void getWavMetaData(const char* inputWavFilePath,
                      char* wavMetaDataJsonStrBuff,
                      unsigned int wavMetaDataJsonStrBuffSize) {

    // Read wav file metadata
    WavMetadata md;
    md.sampleRate = 0;
    md.numChannels = 0;
    md.bitsPerSample = 0;
    md.numSamples = 0;
    WavMetadataReader wavMetadataReader;
    ResultCode rc = wavMetadataReader.readMetadata(inputWavFilePath, &md);
    if (rc != RESULT_CODE_OK) {
      fprintf(stderr, "ERROR: While reading wav file metadata: %d\n", rc);
    }

    sprintf(wavMetaDataJsonStrBuff,
      "{"
        "\"resultCode\":%u,"  // unsigned ok
        "\"wavMetaData\":{"
          "\"sampleRate\":%u,"
          "\"numChannels\":%u,"
          "\"bitsPerSample\":%u,"
          "\"numSamples\":%u"  // ui32 fits WAV size limits
        "}"
      "}", 
      rc,
      md.sampleRate,
      md.numChannels,
      md.bitsPerSample,
      md.numSamples
    );
    
    return;
  }

  int wavToFlac(const char* inputWavFilePath, const char* outputFlacFilePath) {
    return WavToFlacFileConverter::convert(inputWavFilePath, outputFlacFilePath);
  }

  // Example method
  double multiply(double a, double b) {
		return a * b;
	}
  
}
