#include "rn-wav-to-flac.hpp"
#include "WavToFlacFileConverter.hpp"

namespace WavToFlacNameSpace {

  double multiply(double a, double b) {
		return a * b;
	}

  int wavToFlac(const char* inputWavFilePath, const char* outputFlacFilePath) {
    return WavToFlacFileConverter::convert(inputWavFilePath, outputFlacFilePath);
  }

}
