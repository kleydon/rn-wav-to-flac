import * as React from 'react'
import RNFS from 'react-native-fs'

import { StyleSheet, View, Text } from 'react-native'
import { multiply, wavToFlac } from 'rn-wav-to-flac'

const ilog = console.log

export default function App() {

  const cacheDirPath = RNFS.CachesDirectoryPath
  ilog('cacheDirPath: ' + cacheDirPath)
  const inWavFilePath = cacheDirPath + '/16BitStereoWavSample.wav'
  const outFlacFilePath = cacheDirPath + '/out.flac'
  ilog('inWavFilePath: ' + inWavFilePath)
  ilog('outFlacFilePath: ' + outFlacFilePath)

  const [resultA, setResultA] = React.useState<number | undefined>();
  const [resultB, setResultB] = React.useState<number | undefined>();

  React.useEffect(() => {
    multiply(3, 7).then(setResultA)
    wavToFlac(inWavFilePath, outFlacFilePath).then(setResultB)
    console.log('A')
    console.log(multiply(3, 7))
  }, []);

  return (
    <View style={styles.container}>
      <Text>Result A: {resultA}</Text>
      <Text>Result B: {resultB}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
