#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

namespace rnwavtoflac {

  void getWavMetaData(const char* inputWavFilePath,
                      char* wavMetaDataJsonStrBuff,
                      unsigned int wavMetaDataJsonStrBuffSize);

  int wavToFlac(const char* inputWavFilePath, const char* outputFlacFilePath);

  // Example method
  double multiply(double a, double b);
  
}

#endif  // __EXAMPLE_H__
